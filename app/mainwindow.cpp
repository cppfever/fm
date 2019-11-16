#include "mainwindow.h"


namespace nanogui
{
MainWindow::MainWindow(const nanogui::Vector2i &size, const std::string &caption,
                       bool resizable, bool fullscreen, int colorBits,
                       int alphaBits, int depthBits, int stencilBits,
                       int nSamples,
                       unsigned int glMajor, unsigned int glMinor)
    : Win32MainWindow(size, caption, resizable, fullscreen, colorBits, alphaBits, depthBits, stencilBits, nSamples, glMajor, glMinor),
      BackImage<MainWindow>(this)
{
    ::glfwMakeContextCurrent(glfwWindow());

    if(::glewInit() != GLEW_OK)
        throw std::runtime_error("MainWindow: Glew initialization failed.");

    setBackground(Color(255, 255, 0, 255));
    setResizeCallback([&](nanogui::Vector2i)
    {
        performLayout();
        drawAll();
    });

    loadBackImage("icons/icon1.png");

    performLayout();

    mBoxLayout = new BoxLayout(Orientation::Vertical, Alignment::Fill, 10, 10);
    this->setLayout(mBoxLayout);

    mCaption = new Panel(this);
    mToolbar = new Panel(this);
    mStatusbar = new Panel(this);

    mCaption->loadBackImage("icons/icon1.png");
    mToolbar->loadBackImage("icons/icon2.png");
    mStatusbar->loadBackImage("icons/icon3.png");

    mCaption->setHeight(100);
    mToolbar->setHeight(100);
    mStatusbar->setHeight(100);
}

MainWindow::~MainWindow()
{}

bool MainWindow::keyboardEvent(int key, int scancode, int action, int modifiers)
{
    if (Screen::keyboardEvent(key, scancode, action, modifiers))
        return true;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        setVisible(false);
        return true;
    }
    return false;
}

void MainWindow::draw(NVGcontext* ctx)
{    
    //drawBackImage(ctx);
    //Screen::draw(ctx);
    Win32MainWindow::draw(ctx);
}
}//namespace nanogui
