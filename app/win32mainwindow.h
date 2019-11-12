#pragma once

#include <windows.h>
#include "std.h"
#include <nanogui/nanogui.h>
#include "themeex.h"


namespace nanogui
{

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
    }

    ~Win32MainWindow()
    {
        deleteRegions();
    }
    /*
    bool resizeEvent(const Vector2i &size) override
    {
        Screen::resizeEvent(size);
        deleteRegions();
        createRegions();
        return true;
    }*/

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
        Vector2i s = size();

        if(themeex()->MainWindowShape == WidgetShape::Rect)
        {
            mOuterRegion = ::CreateRectRgn(0, 0, s.x(), s.y());
        }
        else
            if(themeex()->MainWindowShape == WidgetShape::RoundRect)
            {
                mOuterRegion = ::CreateRoundRectRgn(0, 0, s.x(), s.y(),
                                                    themeex()->WidgetCornerRadius,
                                                    themeex()->WidgetCornerRadius);
            }
            else
                if(themeex()->MainWindowShape == WidgetShape::TopRoundRect)
                {
                    mOuterRegion = ::CreateRoundRectRgn(0, 0, s.x(), s.y(),
                                                        themeex()->WidgetCornerRadius,
                                                        themeex()->WidgetCornerRadius);

                    HRGN bottomregion = ::CreateRectRgn(0, s.y() - themeex()->WidgetCornerRadius,
                                                        s.x(), s.y());

                    ::CombineRgn(mOuterRegion, mOuterRegion, bottomregion, RGN_OR);
                }

        ::SetWindowRgn(mHwnd, mOuterRegion, TRUE);
        ::UpdateWindow(mHwnd);
    }

    virtual void deleteRegions()
    {
        if(mOuterRegion)
        {
            ::DeleteObject(mOuterRegion);
            mOuterRegion = nullptr;
        }
    }

    friend class Initializer;

    HWND mHwnd {nullptr};
    HRGN mOuterRegion {nullptr};
    HRGN mInnerRegion {nullptr};
/*    HRGN mLeftRegion {nullptr};
    HRGN mOuterRegion {nullptr};
    HRGN mOuterRegion {nullptr};
    HRGN mOuterRegion {nullptr};
    HRGN mOuterRegion {nullptr};
    HRGN mOuterRegion {nullptr};
    HRGN mOuterRegion {nullptr};
    HRGN mOuterRegion {nullptr};*/

    WidgetShape mShape;
    ThemeEx mThemeEx;
};//class Win32MainWindow

}//namespace nanogui
