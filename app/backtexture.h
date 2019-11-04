#pragma once

#include "std.h"
#include <nanogui/nanogui.h>
#include "gltexture.h"


namespace nanogui
{

class BackTexture
{
public:

    BackTexture(Widget *widget) : mWidget(widget)
    {}

    ~BackTexture()
    {
        mBackShader.free();
    }

    virtual void draw(NVGcontext *ctx)
    {
        nvgEndFrame(ctx); // Flush the NanoVG draw stack, not necessary to call nvgBeginFrame afterwards.

        // Calculate several variables that need to be send to OpenGL in order for the image to be
        // properly displayed inside the widget.
        const Screen* screen = mWidget->screen();
        assert(screen);
        //center();
        Vector2f screenSize = screen->size().cast<float>();
        Vector2f scaleFactor = mScale * mBackTexture.sizeF().cwiseQuotient(screenSize);
        Vector2f positionInScreen = mWidget->absolutePosition().cast<float>();
        Vector2f positionAfterOffset = positionInScreen + mOffset;
        Vector2f imagePosition = positionAfterOffset.cwiseQuotient(screenSize);
        glEnable(GL_SCISSOR_TEST);
        float r = screen->pixelRatio();
        glScissor(positionInScreen.x() * r,
                  (screenSize.y() - positionInScreen.y() - mWidget->size().y()) * r,
                  mWidget->size().x() * r, mWidget->size().y() * r);
        mBackShader.bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mBackTexture.texture());
        mBackShader.setUniform("image", 0);
        mBackShader.setUniform("scaleFactor", scaleFactor);
        mBackShader.setUniform("position", imagePosition);
        mBackShader.drawIndexed(GL_TRIANGLES, 0, 2);        
        glDisable(GL_SCISSOR_TEST);
    }

    virtual void load(const std::string& file)
    {
        mBackTexture.load(file);

        mBackShader.init(
                    "BackImageShader",
                    R"(#version 330
                    uniform vec2 scaleFactor;
                    uniform vec2 position;
                    in vec2 vertex;
                    out vec2 uv;
                    void main() {
                    uv = vertex;
                    vec2 scaledVertex = (vertex * scaleFactor) + position;
                    gl_Position  = vec4(2.0*scaledVertex.x - 1.0,
                    1.0 - 2.0*scaledVertex.y,
                    0.0, 1.0);

                    })",
                    R"(#version 330
                    uniform sampler2D image;
                    out vec4 color;
                    in vec2 uv;
                    void main() {
                    color = texture(image, uv);
                    })");

        MatrixXu indices(3, 2);
        indices.col(0) << 0, 1, 2;
        indices.col(1) << 2, 3, 1;

        MatrixXf vertices(2, 4);
        vertices.col(0) << 0, 0;
        vertices.col(1) << 1, 0;
        vertices.col(2) << 0, 1;
        vertices.col(3) << 1, 1;

        mBackShader.bind();
        mBackShader.uploadIndices(indices);
        mBackShader.uploadAttrib("vertex", vertices);
    }

    Vector2f coordinateAt(const Vector2f& position) const
    {
        auto imagePosition = position - mOffset;
        return imagePosition / mScale;
    }

    Vector2f clampedCoordinateAt(const Vector2f& position) const
    {
        auto imageCoordinate = coordinateAt(position);
        return imageCoordinate.cwiseMax(Vector2f::Zero()).cwiseMin(sizeF());
    }

    Vector2f positionForCoordinate(const Vector2f& imageCoordinate) const
    {
        return mScale*imageCoordinate + mOffset;
    }

    void setCoordinateAt(const Vector2f& position, const Vector2f& imageCoordinate)
    {
        // Calculate where the new offset must be in order to satisfy the image position equation.
        // Round the floating point values to balance out the floating point to integer conversions.
        mOffset = position - (imageCoordinate * mScale);

        // Clamp offset so that the image remains near the screen.
        mOffset = mOffset.cwiseMin(sizeF()).cwiseMax(-scaledSizeF());
    }

    //Vector2f positionF() const { return mWidget->position().cast<float>(); }
    //Vector2f sizeF() const { return mWidget->size().cast<float>(); }

    Vector2i size() const { return mBackTexture.size(); }
    Vector2i scaledSize() const { return (mScale * mBackTexture.sizeF()).cast<int>(); }
    Vector2f sizeF() const { return mBackTexture.sizeF(); }
    Vector2f scaledSizeF() const { return (mScale * mBackTexture.sizeF()); }

    const Vector2f& offset() const { return mOffset; }
    void setOffset(const Vector2f& offset) { mOffset = offset; }
    float scale() const { return mScale; }
    void setScale(float scale) { mScale = scale > 0.01f ? scale : 0.01f; }

    bool fixedOffset() const { return mFixedOffset; }
    void setFixedOffset(bool fixedOffset) { mFixedOffset = fixedOffset; }
    bool fixedScale() const { return mFixedScale; }
    void setFixedScale(bool fixedScale) { mFixedScale = fixedScale; }

    float zoomSensitivity() const { return mZoomSensitivity; }
    void setZoomSensitivity(float zoomSensitivity) { mZoomSensitivity = zoomSensitivity; }

    void center()
    {
        Vector2f sz(mWidget->width(), mWidget->height());
        mOffset = (sz - scaledSizeF()) / 2;
    }

    void fit()
    {
        Vector2f sz(mWidget->width(), mWidget->height());
        mScale = (sz.cwiseQuotient(mBackTexture.sizeF())).minCoeff();
        center();
    }

    void setScaleCentered(float scale)
    {
        auto centerPosition = mWidget->size().cast<float>() / 2;
        auto p = coordinateAt(centerPosition);
        mScale = scale;
        setCoordinateAt(centerPosition, p);
    }

    void moveOffset(const Vector2f& delta) {
        // Apply the delta to the offset.
        mOffset += delta;
        auto sizef = mWidget->size().cast<float>();

        // Prevent the image from going out of bounds.
        auto scaled_size = scaledSizeF();
        if (mOffset.x() + scaled_size.x() < 0)
            mOffset.x() = -scaled_size.x();
        if (mOffset.x() > sizef.x())
            mOffset.x() = sizef.x();
        if (mOffset.y() + scaled_size.y() < 0)
            mOffset.y() = -scaled_size.y();
        if (mOffset.y() > sizef.y())
            mOffset.y() = sizef.y();
    }

    void zoom(int amount, const Vector2f& focusPosition)
    {
        auto focusedCoordinate = coordinateAt(focusPosition);
        float scaleFactor = std::pow(mZoomSensitivity, amount);
        mScale = std::max(0.01f, scaleFactor * mScale);
        setCoordinateAt(focusPosition, focusedCoordinate);
    }

protected:

    Widget* mWidget;
    GLShader mBackShader;
    GLTexture mBackTexture;

    float mScale {1.0};
    Vector2f mOffset;
    bool mFixedScale;
    bool mFixedOffset;
    float mZoomSensitivity = 1.1f;
};//class Panel

}//namespace nanogui
