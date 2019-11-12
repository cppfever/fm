#pragma once

#include "std.h"
#include <nanogui/nanogui.h>
#include "themeex.h"
#include "backimage.h"


namespace nanogui
{

class Panel : public Widget, public BackImage<Panel>
{
public:

    Panel(Widget *parent);
    ~Panel();

    void draw(NVGcontext *ctx) override;
    bool mouseEnterEvent(const Vector2i &p, bool enter) override;
    ThemeEx* themeex();

protected:

    ThemeEx* mThemeEx {nullptr};

};//class Panel

}//namespace nanogui
