#pragma once

#include "panel.h"

namespace nanogui
{

class FilePanel : public Panel
{
public:

    FilePanel(Widget *parent) : Panel(parent)
    {
        setDefaultSettings();
    }

    void draw(NVGcontext *ctx) override
    {
        Panel::draw(ctx);
    }

    void setDefaultSettings() override
    {
        Panel::setDefaultSettings();
    }

protected:

};//class FilePanel

}//namespace nanogui
