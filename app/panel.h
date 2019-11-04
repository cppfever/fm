#pragma once

#include "std.h"
#include <nanogui/nanogui.h>


namespace nanogui
{

class Panel : public Widget
{
public:

    Panel(Widget *parent) : Widget(parent)
    {
        setDefaultSettings();
    }

    void draw(NVGcontext *ctx) override
    {
        Widget::draw(ctx);

        nvgBeginPath(ctx);
        nvgRoundedRect(ctx, mPos.x(), mPos.y(), mSize.x(),
                       mSize.y(), mCornerRadius);

        NVGpaint bg = nvgLinearGradient(ctx, mPos.x(), mPos.y(), mPos.x(),
                                        mPos.y() + mSize.y(), mTopGradient, mBottomGradient);

        nvgFillPaint(ctx, bg);
        nvgFill(ctx);
    }

    virtual void setDefaultSettings()
    {
        setFontSize(theme()->mStandardFontSize);
        mPadding = 5;
        mCornerRadius = mTheme->mButtonCornerRadius;
        mTopGradient = Color(0, 0, 255, 59);//mTheme->mButtonGradientTopUnfocused;
        mBottomGradient = mTheme->mButtonGradientBotUnfocused;
        setHeight(mPadding + mFontSize + mPadding);
    }

protected:

    inline static int mPadding = 5;
    int mCornerRadius;
    Color mTopGradient;
    Color mBottomGradient;
};//class Panel

}//namespace nanogui
