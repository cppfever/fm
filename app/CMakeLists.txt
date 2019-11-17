cmake_minimum_required(VERSION 3.1)

project(filemanager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(UNICODE)

if(UNIX)
    message(STATUS "Unix detected for GLEW")
    set(GLEW_LIB GLEW)
elseif(WIN32)
    message(STATUS "Win32 detected for GLEW32")
    set(GLEW_LIB GLEW32)
endif()

add_executable(filemanager main.cpp mainwindow.cpp win32mainwindow.cpp panel.cpp)
target_link_libraries(filemanager nanogui polyclipping nanovg ${GLEW_LIB} ${OPENGL_LIBRARIES} ${NANOGUI_EXTRA_LIBS})

