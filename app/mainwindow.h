#pragma once

#include "std.h"
#include <nanogui/nanogui.h>
#include "gltexture.h"
#include "backimage.h"
#include "panel.h"
#include "win/mainwindow.h"

namespace nanogui
{

class MainWindow : public win::MainWindow, public BackImage
{
public:

    MainWindow(const nanogui::Vector2i &size, const std::string &caption,
               bool resizable = true, bool fullscreen = false, int colorBits = 8,
               int alphaBits = 8, int depthBits = 24, int stencilBits = 8,
               int nSamples = 0,
               unsigned int glMajor = 2, unsigned int glMinor = 1)
        : win::MainWindow(size, caption, resizable, fullscreen, colorBits, alphaBits, depthBits, stencilBits, nSamples),
          BackImage(this)
    {
        ::glfwMakeContextCurrent(glfwWindow());

        if(::glewInit() != GLEW_OK)
            throw std::runtime_error("FM: Glew initialization failed.");

        setBackground(Color(127, 127, 127, 255));
        setResizeCallback([&](nanogui::Vector2i)
        {
            performLayout();
            drawAll();
        });

        loadBackImage("icons/icon1.png");
        mDrawRoundedRect = false;

        performLayout();

        mBoxLayout = new BoxLayout(Orientation::Vertical, Alignment::Fill, 10, 10);
        this->setLayout(mBoxLayout);

        mCaption = new Panel(this);
        mToolbar = new Panel(this);
        mStatusbar = new Panel(this);

        mCaption->loadBackImage("icons/icon1.png");
        mToolbar->loadBackImage("icons/icon2.png");
        mStatusbar->loadBackImage("icons/icon3.png");

        mCaption->setHeight(100);
        mToolbar->setHeight(100);
        mStatusbar->setHeight(100);
    }

    ~MainWindow()
    {}

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
        nvgSave(ctx);
        Widget::draw(ctx);
        drawBackImage(ctx);
        Screen::draw(ctx);
        nvgRestore(ctx);
    }

private:

    BoxLayout* mBoxLayout {nullptr};
    Panel* mCaption {nullptr};
    Panel* mToolbar {nullptr};
    Panel* mStatusbar {nullptr};    
};

}//namespace nanogui
