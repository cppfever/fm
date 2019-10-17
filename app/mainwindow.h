#pragma once

#include <nanogui/screen.h>

NAMESPACE_BEGIN(nanogui)

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
        Screen::performLayout();
    }

    ~MainWindow()
    {

    }

    virtual bool keyboardEvent(int key, int scancode, int action, int modifiers) override
    {
        if (Screen::keyboardEvent(key, scancode, action, modifiers))
            return true;
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            setVisible(false);
            return true;
        }
        return false;
    }

    virtual void draw(NVGcontext *ctx) override
    {
       // Screen::draw(ctx);
    }

    virtual void drawContents() override
    {
    }
private:
};


NAMESPACE_END(nanogui)
