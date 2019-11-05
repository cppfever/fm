#include "main.h"



int main(int /* argc */, char ** /* argv */)
{
    try
    {
        nanogui::init();

        {
//            ::glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
//            ::glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

            auto mainwindow = nanogui::MainWindow(nanogui::Vector2i(800, 600), "FM");
            //::glfwSetWindowPos(mainwindow.glfwWindow(), 300, 300);
            //::glfwSetWindowAttrib(nullptr,0,0);//mainwindow.glfwWindow(), GLFW_DECORATED, GLFW_TRUE);
            mainwindow.setVisible(true);
            mainwindow.theme()->mButtonCornerRadius = 5;

            mainwindow.performLayout();

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
    catch(...)
    {
        std::cerr << "Some exception was caught..." << std::endl;
        return -1;
    }

    return 0;
}
