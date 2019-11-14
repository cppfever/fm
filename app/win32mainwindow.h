#pragma once

#include <windows.h>
#include "std.h"
#include <nanogui/nanogui.h>
#include <clipper.hpp>
#include "themeex.h"


namespace nanogui
{

using namespace ClipperLib;


class Win32MainWindow : public Screen
{
public:

    Win32MainWindow(const nanogui::Vector2i &size, const std::string &caption,
                    bool resizable = true, bool fullscreen = false, int colorBits = 8,
                    int alphaBits = 8, int depthBits = 24, int stencilBits = 8,
                    int nSamples = 0,
                    unsigned int glMajor = 2, unsigned int glMinor = 1)
        : nanogui::Screen(size, caption, resizable, fullscreen, colorBits, alphaBits, depthBits, stencilBits, nSamples),
          mThemeEx(nvgContext())
    {
        mHwnd = ::glfwGetWin32Window(glfwWindow());
        createRegions();
    }

    ~Win32MainWindow()
    {
        deleteRegions();
    }

    bool resizeEvent(const Vector2i &size) override
    {
        Screen::resizeEvent(size);
        deleteRegions();
        createRegions();
        return true;
    }

    bool mouseMotionEvent(const Vector2i &p, const Vector2i &rel, int button, int modifiers) override
    {
        ::UpdateWindow(mHwnd);
        return true;
    }

    ThemeEx* themeex()
    {
        return &mThemeEx;
    }

protected:    

    virtual void createRegions();
    virtual void deleteRegions();
    void createSidePath(ClipperLib::Paths& subject, ClipperLib::Paths& solution, int x0, int y0, int x1, int y1);
    void drawPath(ClipperLib::Path& path);
    void drawPaths(ClipperLib::Paths& paths);
    void draw(NVGcontext* ctx) override;

    HWND mHwnd {nullptr};
    HRGN mOuterRgn {nullptr};
    ThemeEx mThemeEx;
    ClipperLib::Clipper mClipper;
    ClipperLib::Paths mSizing,
    mOuter, mInner,
    mLeft, mTop, mRight, mBottom,
    mLeftTop, mRightTop, mLeftBottom, mRightBottom;
};//class Win32MainWindow

}//namespace nanogui
