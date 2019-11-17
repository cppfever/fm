#pragma once

//std
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <string>
#include <memory>

//GL
//#include <GL/glew.h>
#include <GLAD/glad.h>


//GLFW3
#include <GLFW/glfw3.h>

//Windows platphorm
#ifdef __WIN32__
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#endif

