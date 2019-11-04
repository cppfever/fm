cmake_minimum_required(VERSION 3.1)

project(filemanager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(UNICODE)

add_executable(filemanager main.cpp)
target_link_libraries(filemanager nanogui glew32 ${NANOGUI_EXTRA_LIBS})

