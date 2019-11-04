#pragma once

#include "std.h"
#include <nanogui/nanogui.h>
#include "gltexture.h"
#include "backimage.h"


namespace nanogui
{

class BackImage
{
public:

    BackImage(Widget *widget) : mWidget(widget)
    {}

    int backImage()
    {
        return mBackImage;
    }

    void drawBackImage(NVGcontext *ctx)
    {
        ::nvgBeginPath(ctx);

        float x = mWidget->position().x();
        float y = mWidget->position().y();
        float width = mWidget->width();
        float height = mWidget->height();

        NVGpaint paint {0};

        if(mBackImage)
            paint = ::nvgImagePattern(ctx, x, y, width, height, mAngle, mBackImage, mTransparency);

        if(mDrawRoundedRect)
            ::nvgRoundedRect(ctx, x, y, width, height, mCornerRadius);
        else
            ::nvgRect(ctx, x, y, width, height);

        if(!mBackImage)
            paint = nvgLinearGradient(ctx, x, y, x, y + height, mTopGradient, mBottomGradient);

        nvgFillPaint(ctx, paint);
        nvgFill(ctx);
    }

    void loadBackImage(const char* filename)
    {
        mBackImage = ::nvgCreateImage(mWidget->screen()->nvgContext(), filename, 0);
    }

protected:

    Widget* mWidget {nullptr};
    int mPadding = 5;
    int mCornerRadius {30};
    Color mTopGradient {0, 0, 255, 60};
    Color mBottomGradient {58, 255};
    int mBackImage {0};
    float mAngle {0.0f};
    float mTransparency {0.5f};
    bool mDrawRoundedRect {true};
};//class BackImage

}//namespace nanogui
