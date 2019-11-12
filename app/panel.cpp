#include "panel.h"
#include "mainwindow.h"


namespace nanogui
{
Panel::Panel(Widget *parent) : Widget(parent), BackImage<Panel>(this)
{
    if(MainWindow* mainwindow = dynamic_cast<MainWindow*>(parent))
        mThemeEx = mainwindow->themeex();
    else
        if(Panel* panel = dynamic_cast<Panel*>(parent))
            mThemeEx = panel->themeex();
        else
            throw std::runtime_error("Panel: panel parent must be MainWindow or Panel.");

    setFontSize(theme()->mStandardFontSize);
    setHeight(mFontSize + 2 * themeex()->WidgetPadding);
}

Panel::~Panel()
{}

void Panel::draw(NVGcontext *ctx)
{
    Widget::draw(ctx);
    drawBackImage(ctx);
}

bool Panel::mouseEnterEvent(const Vector2i &p, bool enter)
{
    if(enter)
        screen()->setCaption("ok");
    else
        screen()->setCaption("no");

    return true;
}

ThemeEx* Panel::themeex()
{
    return mThemeEx;
}

}//namespace nanogui
