#include <GL/glew.h>
#include <nanogui/nanogui.h>
#include <iostream>
#include <string>
#include "mainwindow.h"

namespace nanogui
{

class Panel : public nanogui::Widget
{
public:

    Panel(Widget *parent) : nanogui::Widget(parent)
    {}

    void draw(NVGcontext *ctx) override
    {
        Widget::draw(ctx);

        nvgBeginPath(ctx);
        nvgRoundedRect(ctx, mPos.x() + 1, mPos.y() + 1.0f, mSize.x() - 2,
                       mSize.y() - 2, mTheme->mButtonCornerRadius - 1);

        NVGcolor gradTop = mTheme->mButtonGradientTopUnfocused;
        NVGcolor gradBot = mTheme->mButtonGradientBotUnfocused;

        if(mBackgroundColor.w() != 0)
        {
            nvgFillColor(ctx, Color(mBackgroundColor.head<3>(), 1.f));
            nvgFill(ctx);
            double v = 1 - mBackgroundColor.w();
                gradTop.a = gradBot.a = mEnabled ? v : v * .5f + .5f;
        }

        NVGpaint bg = nvgLinearGradient(ctx, mPos.x(), mPos.y(), mPos.x(),
                                        mPos.y() + mSize.y(), gradTop, gradBot);

        nvgFillPaint(ctx, bg);
        nvgFill(ctx);

        nvgBeginPath(ctx);
        nvgStrokeWidth(ctx, 1.0f);
        nvgRoundedRect(ctx, mPos.x() + 0.5f, mPos.y() + 1.5f, mSize.x() - 1,
                       mSize.y() - 1 - 1.0f, mTheme->mButtonCornerRadius);
        nvgStrokeColor(ctx, mTheme->mBorderLight);
        nvgStroke(ctx);

        nvgBeginPath(ctx);
        nvgRoundedRect(ctx, mPos.x() + 0.5f, mPos.y() + 0.5f, mSize.x() - 1,
                       mSize.y() - 2, mTheme->mButtonCornerRadius);
        nvgStrokeColor(ctx, mTheme->mBorderDark);
        nvgStroke(ctx);
    }

private:

    Color mBackgroundColor {255, 0, 0 , 255};
};

}//namespace nanogui




int main(int /* argc */, char ** /* argv */)
{
    try
    {
        nanogui::init();

        {
            auto screen = nanogui::Screen(nanogui::Vector2i(800, 600), "FM");
            screen.setVisible(true);
            screen.theme()->mButtonCornerRadius = 10;
            screen.theme()->mButtonCornerRadius = 3;
            screen.setResizeCallback([&](nanogui::Vector2i){screen.drawAll();});

            auto caption = new nanogui::Panel(&screen);
            //auto caption = new nanogui::Button(&screen);
            caption->setVisible(true);
            caption->setPosition(nanogui::Vector2i(10, 10));
            caption->setFixedSize(nanogui::Vector2i(100, 100));

            auto label = new nanogui::Label(&screen,"labelsssssssssss");
            label->setFontSize(30);
            label->setColor(nanogui::Color(0,0,0,255));
            label->setVisible(true);
            label->setPosition(nanogui::Vector2i(50, 50));
            label->setFixedSize(nanogui::Vector2i(800, 400));

            screen.performLayout();
            screen.drawWidgets();
            screen.drawAll();

            nanogui::mainloop();
        }

        nanogui::shutdown();
    }
    catch (const std::runtime_error &e)
    {
        std::string error_msg = std::string("Caught a fatal error: ") + std::string(e.what());
        std::cerr << error_msg << std::endl;
        return -1;
    }

    return 0;
}
