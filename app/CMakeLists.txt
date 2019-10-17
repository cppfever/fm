cmake_minimum_required(VERSION 3.1)

project(filemanager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(UNICODE)

add_executable(filemanager main.cpp)

target_link_libraries(filemanager nanogui ${NANOGUI_EXTRA_LIBS})

# Compile a target using NanoGUI
#add_executable(myTarget myTarget.cpp)

# Lastly, additional libraries may have been built for you.  In addition to linking
# against NanoGUI, we need to link against those as well.
#target_link_libraries(myTarget nanogui ${NANOGUI_EXTRA_LIBS})
