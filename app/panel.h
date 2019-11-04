#pragma once

#include "std.h"
#include <nanogui/nanogui.h>
#include "gltexture.h"
#include "backimage.h"


namespace nanogui
{

class Panel : public Widget, public BackImage
{
public:

    Panel(Widget *parent) : Widget(parent), BackImage(this)
    {
        setFontSize(theme()->mStandardFontSize);
        setHeight(mPadding + mFontSize + mPadding);
    }

    ~Panel()
    {}

    void draw(NVGcontext *ctx) override
    {
        Widget::draw(ctx);
        drawBackImage(ctx);
    }
};//class Panel

}//namespace nanogui
