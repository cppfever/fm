cmake_minimum_required(VERSION 3.1)

project(filemanager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(UNICODE)

add_executable(filemanager main.cpp mainwindow.cpp win32mainwindow.cpp framebuffer.cpp panel.cpp)
target_link_libraries(filemanager nanogui polyclipping ${OPENGL_LIBRARIES} ${NANOGUI_EXTRA_LIBS})

