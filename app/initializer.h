#pragma once

#include "std.h"
#include <nanogui/nanogui.h>
#include "gltexture.h"
#include "backimage.h"
#include "panel.h"
#include "win/mainwindow.h"

namespace nanogui
{

class Initializer
{
public:

    Initializer()
    {
        nanogui::init();

        //::glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        //::glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        sStatus = true;
    }

    ~Initializer()
    {
        nanogui::shutdown();
    }

    static bool Status()
    {
        return sStatus;
    }

private:

    static inline bool sStatus {false};
} __fm__initializer__;

}//namespace nanogui
