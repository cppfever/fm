#pragma once

#include "std.h"
#include <nanogui/nanogui.h>
#include "backimage.h"
#include "win32mainwindow.h"
#include "themeex.h"
#include "panel.h"


namespace nanogui
{

class MainWindow : public Win32MainWindow, public BackImage<MainWindow>
{
public:

    MainWindow(const nanogui::Vector2i &size, const std::string &caption,
               bool resizable = true, bool fullscreen = false, int colorBits = 8,
               int alphaBits = 8, int depthBits = 24, int stencilBits = 8,
               int nSamples = 0,
               unsigned int glMajor = 3, unsigned int glMinor = 3);
    ~MainWindow();

    bool keyboardEvent(int key, int scancode, int action, int modifiers) override;
    void draw(NVGcontext* ctx) override;

private:

    BoxLayout* mBoxLayout {nullptr};
    Panel* mCaption {nullptr};
    Panel* mToolbar {nullptr};
    Panel* mStatusbar {nullptr};    
};

}//namespace nanogui
