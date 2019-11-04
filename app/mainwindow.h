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

        setBackground(Color(127, 127, 127, 255));
        setResizeCallback([&](nanogui::Vector2i)
        {
            performLayout();
            drawAll();
        });

        mBoxLayout = new BoxLayout(Orientation::Vertical, Alignment::Fill, 10, 10);
        this->setLayout(mBoxLayout);

        mCaption = new Panel(this);
        mToolbar = new Panel(this);
        mStatusbar = new Panel(this);

        mCaption->setHeight(100);
        mToolbar->setHeight(100);
        mStatusbar->setHeight(100);

        performLayout();
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
