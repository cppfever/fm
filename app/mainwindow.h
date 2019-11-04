#pragma once

#include "std.h"
#include <nanogui/nanogui.h>
#include "panel.h"
#include "gltexture.h"

namespace nanogui
{

class MainWindow : public nanogui::Screen
{
public:

    MainWindow(const nanogui::Vector2i &size, const std::string &caption,
               bool resizable = true, bool fullscreen = false, int colorBits = 8,
               int alphaBits = 8, int depthBits = 24, int stencilBits = 8,
               int nSamples = 0,
               unsigned int glMajor = 3, unsigned int glMinor = 3)
        : nanogui::Screen(size, caption, resizable, fullscreen, colorBits, alphaBits, depthBits, stencilBits, nSamples)
    {
        ::glewInit();

        setBackground(Color(0, 0, 127, 255));
        setResizeCallback([&](nanogui::Vector2i)
        {
            performLayout();
            drawAll();
        });        

        mBoxLayout = new BoxLayout(Orientation::Vertical, Alignment::Fill, 10, 10);
        this->setLayout(mBoxLayout);

        mWindow = new Window(this, "");


 /*       mCaption = new Panel(this);
        mToolbar = new Panel(this);
        mStatusbar = new Panel(this);*/

        loadImages();
        mImageview = new ImageView(mWindow, mBackImage.texture());
        performLayout();
        //initShader();
    }

    ~MainWindow()
    {
        //mShader.free();
    }

    bool keyboardEvent(int key, int scancode, int action, int modifiers) override
    {
        if (Screen::keyboardEvent(key, scancode, action, modifiers))
            return true;
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            setVisible(false);
            return true;
        }
        return false;
    }

    void draw(NVGcontext* ctx) override
    {
        //drawShader();
        nvgSave(ctx);
        Screen::draw(ctx);
        nvgRestore(ctx);
    }

    void loadImages()
    {
        mBackImage.load("d:/doc/cpp/fm/fm/icons/icon1.png");
    }

    void initShader()
    {
        mShader.init(
            /* An identifying name */
            "a_simple_shader",

            /* Vertex shader */
            "#version 330\n"
            "uniform mat4 modelViewProj;\n"
            "in vec3 position;\n"
            "void main() {\n"
            "    gl_Position = modelViewProj * vec4(position, 1.0);\n"
            "}",

            /* Fragment shader */
            "#version 330\n"
            "out vec4 color;\n"
            "uniform float intensity;\n"
            "void main() {\n"
            "    color = vec4(vec3(intensity), 1.0);\n"
            "}"
        );

        MatrixXu indices(3, 2); /* Draw 2 triangles */
        indices.col(0) << 0, 1, 2;
        indices.col(1) << 2, 3, 0;

        MatrixXf positions(3, 4);
        positions.col(0) << -1, -1, 0;
        positions.col(1) <<  1, -1, 0;
        positions.col(2) <<  1,  1, 0;
        positions.col(3) << -1,  1, 0;

        mShader.bind();
        mShader.uploadIndices(indices);
        mShader.uploadAttrib("position", positions);
        mShader.setUniform("intensity", 0.5f);
    }

    void drawShader()
    {
        mShader.bind();

        Matrix4f mvp;
        mvp.setIdentity();
        mvp.topLeftCorner<3,3>() = Matrix3f(Eigen::AngleAxisf((float) glfwGetTime(),  Vector3f::UnitZ())) * 0.25f;

        mvp.row(0) *= (float) mSize.y() / (float) mSize.x();

        mShader.setUniform("modelViewProj", mvp);

        /* Draw 2 triangles starting at index 0 */
        mShader.drawIndexed(GL_TRIANGLES, 0, 2);
    }

private:

    Window* mWindow {nullptr};
    BoxLayout* mBoxLayout {nullptr};
    Panel* mCaption {nullptr};
    Panel* mToolbar {nullptr};
    Panel* mStatusbar {nullptr};

    GLShader mShader;
    GLTexture mBackImage;
    ImageView* mImageview {nullptr};
};

}//namespace nanogui
