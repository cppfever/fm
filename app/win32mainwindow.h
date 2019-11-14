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
               << IntPoint(width - xthick , ythick )
               << IntPoint(width - xthick , height - ythick)
               << IntPoint(xthick, height - ythick);

        ClipperLib::Clipper clipper;
        clipper.AddPath(mOuter, ClipperLib::ptSubject, true);
        clipper.AddPath(mInner, ClipperLib::ptClip, true);
        clipper.Execute(ClipperLib::ctDifference, mSizing,
                        ClipperLib::pftEvenOdd, ClipperLib::pftEvenOdd);
        //::SetWindowRgn(mHwnd, mOuterRgn, TRUE);
        //::UpdateWindow(mHwnd);
    }

    virtual void deleteRegions()
    {
        if(mOuterRgn)
        {
            ::DeleteObject(mOuterRgn);
            mOuterRgn = nullptr;
        }

        mOuter.clear();
        mInner.clear();
        mSizing.clear();
    }

    void drawPath(ClipperLib::Path& path, Color color)
    {        
        NVGcontext* ctx = nvgContext();

        ::nvgBeginPath(ctx);

        ::nvgMoveTo(ctx, path[0].X, path[0].Y);
        for(size_t i = 1; i < path.size(); i++)
            ::nvgLineTo(ctx, path[i].X, path[i].Y);

        ::nvgClosePath(ctx);

        ::nvgFillColor(ctx, color);
        ::nvgFill(ctx);
    }

    void drawPaths(ClipperLib::Paths& paths, Color color)
    {
        NVGcontext* ctx = nvgContext();

        for(size_t I = 0; I < paths.size(); I++)
        {
            ::nvgBeginPath(ctx);

            ::nvgMoveTo(ctx, paths[I][0].X, paths[I][0].Y);

            for(size_t i = 1; i < paths[I].size(); i++)
                ::nvgLineTo(ctx, paths[I][i].X, paths[I][i].Y);

            ::nvgClosePath(ctx);

            ::nvgFillColor(ctx, color);
            ::nvgFill(ctx);
        }
    }

    void draw(NVGcontext* ctx) override
    {
        drawPaths(mSizing,Color(255, 0, 0, 255));
    }

    HWND mHwnd {nullptr};
    HRGN mOuterRgn {nullptr};
    ThemeEx mThemeEx;
    ClipperLib::Paths mSizing;
    ClipperLib::Path mOuter, mInner,
    mLeft, mTop, mRight, mBottom,
    mLeftTop, mRightTop, mLeftBottom, mRightBottom;
};//class Win32MainWindow

}//namespace nanogui
