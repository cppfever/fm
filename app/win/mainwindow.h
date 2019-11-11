#pragma once

#include "../std.h"
#include <windows.h>
#include "../themeex.h"
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
        if(!sTheme)
            sTheme = new ThemeEx(nvgContext());

        setTheme(sTheme);
        setShape(WidgetShape::TopRoundRect);
        mHwnd = ::glfwGetWin32Window(glfwWindow());
    }

    ~MainWindow()
    {
        deleteRegions();
    }

    bool resizeEvent(const Vector2i &size) override
    {
        deleteRegions();
        createRegions();
        return true;
    }

    bool mouseMotionEvent(const Vector2i &p, const Vector2i &rel, int button, int modifiers) override
    {
        ::UpdateWindow(mHwnd);
        return true;
    }

    void setShape(WidgetShape shape)
    {
        mShape = shape;
        createRegions();
    }

protected:    

    virtual void createRegions()
    {
        ThemeEx* t = dynamic_cast<ThemeEx*>(theme());
        Vector2i s = size();

        if(mShape == WidgetShape::Rect)
        {
            mOuterRegion = ::CreateRectRgn(0, 0, s.x(), s.y());
        }
        else
            if(mShape == WidgetShape::RoundRect)
            {
                mOuterRegion = ::CreateRoundRectRgn(0, 0, s.x(), s.y(),
                                                            sTheme->WidgetCornerRadius,
                                                            sTheme->WidgetCornerRadius);
            }
            else
                if(mShape == WidgetShape::TopRoundRect)
                {
                    mOuterRegion = ::CreateRoundRectRgn(0, 0, s.x(), s.y(),
                                                                sTheme->WidgetCornerRadius,
                                                                sTheme->WidgetCornerRadius);

                    HRGN bottomregion = ::CreateRectRgn(0, s.y() - sTheme->WidgetCornerRadius,
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

    HWND mHwnd {nullptr};
    HRGN mOuterRegion {nullptr};
    WidgetShape mShape;

    static inline ref<ThemeEx> sTheme;
};

}//namespace nanogui::win
