#pragma once

#include "std.h"
#include <nanogui/nanogui.h>


namespace nanogui
{

enum class WidgetShape
{
    Rect,
    RoundRect,
    TopRoundRect
};

class ThemeEx : public Theme
{
public:

    ThemeEx(NVGcontext* ctx) : Theme(ctx)
    {
        DefaultWidgetShape = WidgetShape::TopRoundRect;
        MainWindowShape = WidgetShape::TopRoundRect;
        WidgetCornerRadius = 100;
        WidgetShadowSize = 5;
        ResizeWidth = 5;
    }

    WidgetShape DefaultWidgetShape;
    WidgetShape MainWindowShape;
    std::uint32_t WidgetCornerRadius;
    std::uint32_t WidgetShadowSize;
    std::uint32_t ResizeWidth;

protected:

    ~ThemeEx() override
    {

    }
};//class ThemeEx

}//namespace nanogui
