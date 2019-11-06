#pragma once

#include "../std.h"
#include <windows.h>
#include "../gltexture.h"
#include "../backimage.h"
#include "../panel.h"

namespace nanogui::win
{

class MainWindow : public nanogui::Screen
{
public:

    MainWindow(const nanogui::Vector2i &size, const std::string &caption,
               bool resizable = true, bool fullscreen = false, int colorBits = 8,
               int alphaBits = 8, int depthBits = 24, int stencilBits = 8,
               int nSamples = 0,
               unsigned int glMajor = 2, unsigned int glMinor = 1)
        : nanogui::Screen(size, caption, resizable, fullscreen, colorBits, alphaBits, depthBits, stencilBits, nSamples)
    {
        hWhd = ::glfwGetWin32Window(glfwWindow());

    }

    ~MainWindow()
    {}

 /*    bool resizeEvent(const Vector2i &size) override
     {



     }*/

protected:

    HWND hWhd {nullptr};
};

}//namespace nanogui::win
