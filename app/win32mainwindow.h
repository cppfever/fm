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

    virtual void createRegions()
    {
        int width = size().x();
        int height = size().y();

        mOuter << IntPoint(0, 0) << IntPoint(width, 0)
               << IntPoint(width, height) << IntPoint(0, height);

        int xthick = themeex()->MainWindowResizeThickness;
        int ythick = themeex()->MainWindowResizeThickness;

        if(3 * xthick > width)
            xthick = width / 3;

        if(3 * ythick > height)
            ythick = height / 3;

        int x2thick = xthick * 2;
        int y2thick = ythick * 2;

        mInner << IntPoint(xthick, ythick)
               << IntPoint(width - x2thick , ythick )
               << IntPoint(width - x2thick , height - y2thick)
               << IntPoint(xthick, height - y2thick);

        ::SetWindowRgn(mHwnd, mOuterRgn, TRUE);
        ::UpdateWindow(mHwnd);
    }

    virtual void deleteRegions()
    {
        if(mOuterRgn)
        {
            ::DeleteObject(mOuterRgn);
            mOuterRgn = nullptr;
        }
    }

    void drawPath(ClipperLib::Path& path, Color color)
    {
        if(path.size() < 2)
            return;

        NVGcontext* ctx = nvgContext();
        ::nvgSave(ctx);
        ::nvgBeginPath(ctx);
        NVGpaint paint{0};

        ::nvgMoveTo(ctx, path[0].X, path[0].Y);
        for(size_t i = 1; i < path.size(); i++)
            ::nvgLineTo(ctx, path[i].X, path[i].Y);

        ::nvgClosePath(ctx);

        //::nvgFillColor(ctx, color);
        ::nvgFill(ctx);
        ::nvgRestore(ctx);
    }

    void draw(NVGcontext* ctx) override
    {
        drawPath(mInner, Color(127, 0, 0, 127));
    }

    HWND mHwnd {nullptr};
    HRGN mOuterRgn {nullptr};
    ThemeEx mThemeEx;
    ClipperLib::Path mOuter, mInner,
    mLeft, mTop, mRight, mBottom,
    mLeftTop, mRightTop, mLeftBottom, mRightBottom;
};//class Win32MainWindow

}//namespace nanogui
