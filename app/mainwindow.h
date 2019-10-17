#pragma once

#include <nanogui/screen.h>
#include "panel.h"

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
        setResizeCallback([&](nanogui::Vector2i)
        {
            performLayout();
            drawAll();
        });

        m_boxlayout = new BoxLayout(Orientation::Vertical, Alignment::Fill, 10, 10);
        this->setLayout(m_boxlayout);
        m_caption = new Panel(this);
        m_caption->setHeight(20);

        m_toolbar = new Panel(this);
        m_toolbar->setHeight(20);

        m_statusbar = new Panel(this);
        m_statusbar->setHeight(20);

        performLayout();
    }

    ~MainWindow()
    {

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

    /*   void performLayout(NVGcontext *ctx) override
    {
        performLayout();
    }*/

    void draw(NVGcontext *ctx) override
    {
        Screen::draw(ctx);
    }

private:

    BoxLayout* m_boxlayout;
    Panel* m_caption;
    Panel* m_toolbar;
    Panel* m_statusbar;

};

}//namespace nanogui
