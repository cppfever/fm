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
    void createSidePath(Paths& solution, int x0, int y0, int x1, int y1);
    void createCornerPath(Paths& solution, Paths& diff1, Paths& diff2, int x0, int y0, int x1, int y1, int x2, int y2,
                           int x3, int y3, int x4, int y4, int x5, int y5);

    void combinePaths(Paths& solution, Paths& subject, Paths& clip, ClipType op);
    void drawPath(Path& path);
    void drawPaths(Paths& paths, Color color);
    void draw(NVGcontext* ctx) override;

    HWND mHwnd {nullptr};
    HRGN mOuterRgn {nullptr};
    ThemeEx mThemeEx;
    Clipper mClipper;
    Paths mSizing,
    mOuter, mInner,
    mLeft, mTop, mRight, mBottom,
    mLeftTop, mRightTop, mLeftBottom, mRightBottom;
    bool mDrawSizingPaths {false};
    bool mDrawHitTest {false};

};//class Win32MainWindow

}//namespace nanogui
