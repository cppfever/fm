#pragma once

#include "std.h"
#include <nanogui/nanogui.h>
#include "gltexture.h"


namespace nanogui
{

class BackImage
{
public:

    BackImage(Widget *widget) : mWidget(widget)
    {}

    ~BackImage()
    {
        mBackShader.free();
    }

    virtual void drawBackImage(NVGcontext *ctx)
    {
        nvgEndFrame(ctx); // Flush the NanoVG draw stack, not necessary to call nvgBeginFrame afterwards.

        // Calculate several variables that need to be send to OpenGL in order for the image to be
        // properly displayed inside the widget.
        const Screen* screen = mWidget->screen();
        assert(screen);
        Vector2f screenSize = screen->size().cast<float>();
        Vector2f scaleFactor = 1.0 * mBackImage.sizeF().cwiseQuotient(screenSize);
        Vector2f positionInScreen = mWidget->absolutePosition().cast<float>();
        Vector2f positionAfterOffset = positionInScreen;
        Vector2f imagePosition = positionAfterOffset.cwiseQuotient(screenSize);
        glEnable(GL_SCISSOR_TEST);
        float r = screen->pixelRatio();
        glScissor(positionInScreen.x() * r,
                  (screenSize.y() - positionInScreen.y() - mWidget->size().y()) * r,
                  mWidget->size().x() * r, mWidget->size().y() * r);
        mBackShader.bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mBackImage.texture());
        mBackShader.setUniform("image", 0);
        mBackShader.setUniform("scaleFactor", scaleFactor);
        mBackShader.setUniform("position", imagePosition);
        mBackShader.drawIndexed(GL_TRIANGLES, 0, 2);
        glDisable(GL_SCISSOR_TEST);
    }

    virtual void loadBackImage(const std::string& file)
    {
        mBackImage.load(file);

        glBindTexture(GL_TEXTURE_2D, mBackImage.texture());
        GLint w, h;
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
        mBackImageSize = Vector2i(w, h);

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


protected:

    Widget* mWidget;
    GLShader mBackShader;
    GLTexture mBackImage;
    Vector2i mBackImageSize;

};//class Panel

}//namespace nanogui
