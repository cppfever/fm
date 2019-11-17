#pragma once

//Windows platphorm
#ifdef __WIN32__
    #include <windows.h>
    #define GLFW_EXPOSE_NATIVE_WIN32
#endif

#define NANOGUI_USE_GLAD 1
#include <GLAD/glad.h>
#include <nanogui/nanogui.h>
#include <GLFW/glfw3native.h>
#include <clipper.hpp>

