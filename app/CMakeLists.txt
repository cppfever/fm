cmake_minimum_required(VERSION 3.1)

project(editor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(UNICODE)

add_executable(editor main.cpp)

target_link_libraries(editor nanogui ${NANOGUI_EXTRA_LIBS})

# Compile a target using NanoGUI
#add_executable(myTarget myTarget.cpp)

# Lastly, additional libraries may have been built for you.  In addition to linking
# against NanoGUI, we need to link against those as well.
#target_link_libraries(myTarget nanogui ${NANOGUI_EXTRA_LIBS})
