#pragma once

#include "std.h"
#include <nanogui/nanogui.h>
#include "gltexture.h"
#include "backimage.h"


namespace nanogui
{

class Panel : public Widget, BackImage
{
public:

    Panel(Widget *parent) : Widget(parent), BackImage(this)
    {
        setDefaultSettings();
    }

    ~Panel()
    {
        mBackShader.free();
    }

    void draw(NVGcontext *ctx) override
    {
        Widget::draw(ctx);

        drawBackImage(ctx);

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
        loadBackImage("icons/icon1.png");
    }

protected:

    inline static int mPadding = 5;
    int mCornerRadius;
    Color mTopGradient;
    Color mBottomGradient;

};//class Panel

}//namespace nanogui
