#pragma once

#include "common.h"


namespace nanogui
{

class FilePanel : public Panel
{
public:

    FilePanel(Widget *parent) : Panel(parent)
    {
    }

    void draw(NVGcontext *ctx) override
    {
        Panel::draw(ctx);
    }

protected:

};//class FilePanel

}//namespace nanogui
