#pragma once

#include <windows.h>
#include "common.h"


namespace fm
{

enum class WidgetShape
{
    Rect,
    RoundRect,
    TopRoundRect
};

class ThemeEx
{
public:

    ThemeEx()
    {
        MainWindowResizeWidth = ::GetSystemMetrics(SM_CXFRAME);
        MainWindowCornerResizeWidth = 3 * MainWindowResizeWidth;
        CursorWidth = ::GetSystemMetrics(SM_CXCURSOR);
        CursorHeight = ::GetSystemMetrics(SM_CYCURSOR);
        IconWidth = ::GetSystemMetrics(SM_CXICON);
        IconHeight = ::GetSystemMetrics(SM_CYICON);
    }

    WidgetShape MainWindowShape {WidgetShape::Rect};
    std::uint32_t MainWindowResizeWidth;
    std::uint32_t MainWindowCornerResizeWidth;

    WidgetShape WidgetShape {WidgetShape::RoundRect};
    std::uint32_t WidgetPadding {5};
    nanogui::Color WidgetTopGradient {0, 0, 255, 60};
    nanogui::Color WidgetBottomGradient {58, 255};
    float WidgetTransparency {0.5f};
    std::uint32_t WidgetCornerRadius {15};
    std::uint32_t WidgetShadowSize {5};
    std::uint32_t WidgetBackImageAngle {0};

    std::uint32_t CursorWidth;
    std::uint32_t CursorHeight;
    std::uint32_t IconWidth;
    std::uint32_t IconHeight;

    static const ThemeEx Default;
};//class ThemeEx

//const ThemeEx ThemeEx::Default;
}//namespace fm
