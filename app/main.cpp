#include "main.h"

using namespace nanogui;

int main(int /* argc */, char ** /* argv */)
{
    try
    {
            auto mainwindow = MainWindow(Vector2i(800, 600), "FM");
            ::glfwSetWindowPos(mainwindow.glfwWindow(), 300, 300);
            mainwindow.setVisible(true);
            mainwindow.performLayout();
            mainloop();
    }
    catch (const std::runtime_error& e)
    {
        std::string error_msg = std::string("Runtime error: ") + std::string(e.what());
        std::cerr << error_msg << std::endl;
        return -1;
    }
    catch (const nanogui::Exception& e)
    {
        e.Print();
        return -2;
    }
    catch(...)
    {
        std::cerr << "Undefined exception was caught..." << std::endl;
        return -3;
    }

    return 0;
}
