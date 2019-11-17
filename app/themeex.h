#pragma once

#include "common.h"


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
    {}

    WidgetShape MainWindowShape {WidgetShape::Rect};
    std::uint32_t MainWindowResizeThickness {5};
    std::uint32_t MainWindowResizeSize {15};

    WidgetShape WidgetShape {WidgetShape::RoundRect};
    std::uint32_t WidgetPadding {5};
    Color WidgetTopGradient {0, 0, 255, 60};
    Color WidgetBottomGradient {58, 255};
    float WidgetTransparency {0.5f};
    std::uint32_t WidgetCornerRadius {15};
    std::uint32_t WidgetShadowSize {5};
    std::uint32_t WidgetBackImageAngle {0};

};//class ThemeEx

}//namespace nanogui
