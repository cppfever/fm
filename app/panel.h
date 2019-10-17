#pragma once

#include <nanogui/nanogui.h>
#include <nanogui/widget.h>

namespace nanogui
{

class Panel : public nanogui::Widget
{
public:

    Panel(Widget *parent) : nanogui::Widget(parent)
    {}

    void draw(NVGcontext *ctx) override
    {
        Widget::draw(ctx);

        nvgBeginPath(ctx);
        nvgRoundedRect(ctx, mPos.x() + 1, mPos.y() + 1.0f, mSize.x() - 2,
                       mSize.y() - 2, mTheme->mButtonCornerRadius - 1);

        NVGcolor gradTop = mTheme->mButtonGradientTopUnfocused;
        NVGcolor gradBot = mTheme->mButtonGradientBotUnfocused;

        if(mBackgroundColor.w() != 0)
        {
            nvgFillColor(ctx, Color(mBackgroundColor.head<3>(), 1.f));
            nvgFill(ctx);
            double v = 1 - mBackgroundColor.w();
                gradTop.a = gradBot.a = mEnabled ? v : v * .5f + .5f;
        }

        NVGpaint bg = nvgLinearGradient(ctx, mPos.x(), mPos.y(), mPos.x(),
                                        mPos.y() + mSize.y(), gradTop, gradBot);

        nvgFillPaint(ctx, bg);
        nvgFill(ctx);

        nvgBeginPath(ctx);
        nvgStrokeWidth(ctx, 1.0f);
        nvgRoundedRect(ctx, mPos.x() + 0.5f, mPos.y() + 1.5f, mSize.x() - 1,
                       mSize.y() - 1 - 1.0f, mTheme->mButtonCornerRadius);
        nvgStrokeColor(ctx, mTheme->mBorderLight);
        nvgStroke(ctx);

        nvgBeginPath(ctx);
        nvgRoundedRect(ctx, mPos.x() + 0.5f, mPos.y() + 0.5f, mSize.x() - 1,
                       mSize.y() - 2, mTheme->mButtonCornerRadius);
        nvgStrokeColor(ctx, mTheme->mBorderDark);
        nvgStroke(ctx);
    }

private:

    nanogui::Color mBackgroundColor {255, 0, 0 , 255};
};

}//namespace nanogui
