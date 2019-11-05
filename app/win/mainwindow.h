#pragma once

#include "../std.h"
#include "../gltexture.h"
#include "../backimage.h"
#include "../panel.h"

namespace nanogui::win
{

class MainWindow : public nanogui::Screen, public nanogui::BackImage
{
public:

    MainWindow(const nanogui::Vector2i &size, const std::string &caption,
               bool resizable = true, bool fullscreen = false, int colorBits = 8,
               int alphaBits = 8, int depthBits = 24, int stencilBits = 8,
               int nSamples = 0,
               unsigned int glMajor = 3, unsigned int glMinor = 3)
        : nanogui::Screen(size, caption, resizable, fullscreen, colorBits, alphaBits, depthBits, stencilBits, nSamples),
          nanogui::BackImage(this)
    {
    }

    ~MainWindow()
    {}

};

}//namespace nanogui::win
