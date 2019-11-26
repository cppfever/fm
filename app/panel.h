#pragma once

#include "common.h"
#include "backimage.h"
#include "themeex.h"


namespace fm
{

class Panel : public nanogui::Widget, public fm::BackImage<fm::Panel>
{
public:

    Panel(nanogui::Widget *parent);
    ~Panel();

    void draw(NVGcontext *ctx) override;
    bool mouseEnterEvent(const nanogui::Vector2i &p, bool enter) override;
    fm::ThemeEx* themeex();

protected:

    fm::ThemeEx* mThemeEx {nullptr};

};//class Panel

}//namespace fm
