#pragma once

#include "std.h"
#include <nanogui/nanogui.h>
#include "themeex.h"


namespace nanogui
{

template<typename T>
class BackImage
{
public:

    BackImage(T* widget) : mBackWidget(widget)
    {}

    ~BackImage()
    {
        deleteBackImage();
    }

    int backImage()
    {
        return mBackImage;
    }

    void drawBackImage(NVGcontext *ctx)
    {
        ::nvgBeginPath(ctx);

        float x = mBackWidget->position().x();
        float y = mBackWidget->position().y();
        float width = mBackWidget->width();
        float height = mBackWidget->height();

        NVGpaint paint{0};

        if(mBackImage)
            paint = ::nvgImagePattern(ctx, x, y, width, height,
                                      mBackWidget->themeex()->WidgetBackImageAngle,
                                      mBackImage,
                                      mBackWidget->themeex()->WidgetTransparency);

        if(mBackWidget->parent() == nullptr)//MainWindow is always rect
            ::nvgRect(ctx, x, y, width, height);
        else
        {
            if(mBackWidget->themeex()->WidgetShape == WidgetShape::RoundRect)
                ::nvgRoundedRect(ctx, x, y, width, height, mBackWidget->themeex()->WidgetCornerRadius);
            else
                ::nvgRect(ctx, x, y, width, height);
        }

        if(!mBackImage)
            paint = ::nvgLinearGradient(ctx, x, y, x, y + height,
                                      mBackWidget->themeex()->WidgetTopGradient,
                                      mBackWidget->themeex()->WidgetBottomGradient);

        ::nvgFillPaint(ctx, paint);
        ::nvgFill(ctx);
    }

    void loadBackImage(const char* filename)
    {
        mBackImage = ::nvgCreateImage(mBackWidget->screen()->nvgContext(), filename, 0);
    }

    void deleteBackImage()
    {
        if(mBackImage)
        {
            ::nvgDeleteImage(mBackWidget->screen()->nvgContext(), mBackImage);
            mBackImage = 0;
        }

    }

private:

    int mBackImage {0};
    T* mBackWidget {nullptr};

};//class BackImage

}//namespace nanogui
