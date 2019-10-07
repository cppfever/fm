#include <nanogui/nanogui.h>
#include <iostream>
#include <string>
#include "toolwindow.h"

/*
class DockWidget : public nanogui::Window
{
public:

    Window( nanogui::Widget* parent) :  nanogui::Window(parent)
    {

    }

private:

    std::vector<nanogui::Window*> mLeft;
    std::vector<nanogui::Window*> mTop;
    std::vector<nanogui::Window*> mRight;
    std::vector<nanogui::Window*> mBottom;

};//class DockWidget

class MainWindow : public nanogui::Screen
{
public:
    MainWindow() : nanogui::Screen(nanogui::Vector2i(1024, 768), "NanoGUI Test")
    {
        using namespace nanogui;

        //        Window *window = new Window(this, "Button demo");
        //        window->setPosition(Vector2i(15, 15));
        //        window->setLayout(new GroupLayout());

        //         No need to store a pointer, the data structure will be automatically
        //           freed when the parent window is deleted */
        //        new Label(window, "Push buttons", "sans-bold");

        //        Button *b = new Button(window, "Plain button");
        //        b->setCallback([] { std::cout << "pushed!" << std::endl; });
        //        b->setTooltip("short tooltip");
//        ToolWindow * window = new ToolWindow(this, "Tool widgets");
//        /////////////////
//        //window->setLayout(window->buttonPanel()->)

//        /////////////////
//        window->setPosition(Vector2i(0,0));
//        window->setFixedSize(nanogui::Vector2i(800, 600));
//        window->setLayout(new GroupLayout());

//        TabWidget* tabWidget = window->add<TabWidget>();
//        tabWidget->setSize(nanogui::Vector2i(600, 400));

//        Widget* layer = tabWidget->createTab("Color Wheel");
//        layer->setLayout(new GroupLayout());

//        // Use overloaded variadic add to fill the tab widget with Different tabs.
//        layer->add<Label>("Color wheel widget", "sans-bold");
//        layer->add<ColorWheel>();

//        layer = tabWidget->createTab("Function Graph");
//        layer->setLayout(new GroupLayout());

//        layer->add<Label>("Function graph widget", "sans-bold");

//        Graph *graph = layer->add<Graph>("Some Function");

//        graph->setHeader("E = 2.35e-3");
//        graph->setFooter("Iteration 89");
//        VectorXf &func = graph->values();
//        func.resize(100);
//        for (int i = 0; i < 100; ++i)
//            func[i] = 0.5f * (0.5f * std::sin(i / 10.f) +
//                              0.5f * std::cos(i / 23.f) + 1);

//        // Dummy tab used to represent the last tab button.
//        tabWidget->createTab("+");

//        // A simple counter.
//        int counter = 1;
//        tabWidget->setCallback([tabWidget, this, counter] (int index) mutable
//        {
//            if (index == (tabWidget->tabCount()-1))
//            {
//                // When the "+" tab has been clicked, simply add a new tab.
//                std::string tabName = "Dynamic " + std::to_string(counter);
//                Widget* layerDyn = tabWidget->createTab(index, tabName);
//                layerDyn->setLayout(new GroupLayout());
//                layerDyn->add<Label>("Function graph widget", "sans-bold");
//                Graph *graphDyn = layerDyn->add<Graph>("Dynamic function");

//                graphDyn->setHeader("E = 2.35e-3");
//                graphDyn->setFooter("Iteration " + std::to_string(index*counter));
//                VectorXf &funcDyn = graphDyn->values();
//                funcDyn.resize(100);
//                for (int i = 0; i < 100; ++i)
//                    funcDyn[i] = 0.5f *
//                            std::abs((0.5f * std::sin(i / 10.f + counter) +
//                                      0.5f * std::cos(i / 23.f + 1 + counter)));
//                ++counter;
//                // We must invoke perform layout from the screen instance to keep everything in order.
//                // This is essential when creating tabs dynamically.
//                performLayout();
//                // Ensure that the newly added header is visible on screen
//                tabWidget->ensureTabVisible(index);

//            }
//        });
//        performLayout();
//    }

//    ~MainWindow()
//    {

//    }

//    virtual bool keyboardEvent(int key, int scancode, int action, int modifiers) {
//        if (Screen::keyboardEvent(key, scancode, action, modifiers))
//            return true;
//        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
//            setVisible(false);
//            return true;
//        }
//        return false;
//    }

//    virtual void draw(NVGcontext *ctx)
//    {
//        Screen::draw(ctx);
//    }

//    virtual void drawContents()
//    {
//    }
//private:

//    nanogui::Window* mDock;
//};

int main(int /* argc */, char ** /* argv */)
{
    try
    {
        nanogui::init();

        {
            nanogui::ref<nanogui::ToolWindow> app = new nanogui::ToolWindow();

            app->drawAll();
            app->setVisible(true);
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
