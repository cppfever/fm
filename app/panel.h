#pragma once

#include "std.h"
#include <nanogui/nanogui.h>
#include "gltexture.h"
#include "backimage.h"


namespace nanogui
{

class Panel : public Widget
{
public:

    Panel(Widget *parent) : Widget(parent), mBackImage(this)
    {
        setDefaultSettings();
    }

    ~Panel()
    {}

    BackImage& backImage()
    {
        return mBackImage;
    }

    void draw(NVGcontext *ctx) override
    {
        Widget::draw(ctx);


       glEnable(GL_SCISSOR_TEST);
        nvgBeginPath(ctx);

        nvgRoundedRect(ctx, mPos.x(), mPos.y(), mSize.x(),
                       mSize.y(), mCornerRadius);

        mBackImage.draw(ctx);
        NVGpaint bg = nvgLinearGradient(ctx, mPos.x(), mPos.y(), mPos.x(),
                                        mPos.y() + mSize.y(), mTopGradient, mBottomGradient);

        nvgFillPaint(ctx, bg);
        nvgFill(ctx);
        glDisable(GL_SCISSOR_TEST);
    }

    virtual void setDefaultSettings()
    {
        setFontSize(theme()->mStandardFontSize);
        mPadding = 5;
        mCornerRadius = 30;//mTheme->mButtonCornerRadius;
        mTopGradient = Color(0, 0, 255, 59);//mTheme->mButtonGradientTopUnfocused;
        mBottomGradient = mTheme->mButtonGradientBotUnfocused;
        setHeight(mPadding + mFontSize + mPadding);
        mBackImage.load("icons/icon1.png");
    }

protected:

    inline static int mPadding = 5;
    int mCornerRadius;
    Color mTopGradient;
    Color mBottomGradient;
    BackImage mBackImage;

};//class Panel

}//namespace nanogui
