#include <GL/glew.h>
#include <nanogui/nanogui.h>
#include <iostream>
#include <string>
#include "toolwindow.h"

using namespace nanogui;

class MainWindow : public nanogui::Screen
{
public:
    MainWindow(const nanogui::Vector2i &size, const std::string &caption,
               bool resizable = true, bool fullscreen = false, int colorBits = 8,
               int alphaBits = 8, int depthBits = 24, int stencilBits = 8,
               int nSamples = 0,
               unsigned int glMajor = 3, unsigned int glMinor = 3)
               : nanogui::Screen(size, caption, resizable, fullscreen, colorBits, alphaBits, depthBits, stencilBits, nSamples)
    {
        performLayout();
    }

    ~MainWindow()
    {

    }

    virtual bool keyboardEvent(int key, int scancode, int action, int modifiers) {
        if (Screen::keyboardEvent(key, scancode, action, modifiers))
            return true;
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            setVisible(false);
            return true;
        }
        return false;
    }

    virtual void draw(NVGcontext *ctx)
    {
        Screen::draw(ctx);
    }

    virtual void drawContents()
    {
    }
private:
};


int main(int /* argc */, char ** /* argv */)
{
    try
    {
        nanogui::init();

        {
            auto window = MainWindow(Vector2i(800, 600), "FM");
            window.setVisible(true);
            window.setResizeCallback([&](const Vector2i& v)
            {
                window.drawAll();
            });

            Theme* t(window.theme());
            t->mDropShadow = Color(1.0f, 0.0f, 0.0f, 1.0f);
            window.setBackground(Color(1.0f, 1.0f, 1.0f, 1.0f));

            ref<Window> panel = new Window(&window, "Panel");

            panel->setFixedSize(Vector2i(400, 400));
            panel->setVisible(true);

            window.performLayout();
            window.drawAll();

            mainloop();
        }

        shutdown();
    }
    catch (const std::runtime_error &e)
    {
        std::string error_msg = std::string("Caught a fatal error: ") + std::string(e.what());
        std::cerr << error_msg << std::endl;
        return -1;
    }

    return 0;
}
