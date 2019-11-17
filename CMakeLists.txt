cmake_minimum_required(VERSION 3.11)
project(fm)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(
    #UNICODE
    NANOVG_GL3_IMPLEMENTATION)

# Always build dll
set(BUILD_SHARED_LIBS ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Find dependencies
find_package(OpenGL REQUIRED)

# Directory where exe,dll and static library saved
set(FM_BINARY_DIR ${CMAKE_BINARY_DIR}/bin-${CMAKE_CXX_COMPILER_ID}-${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${FM_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${FM_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${FM_BINARY_DIR})

########### NanoGUI ################
set(NANOGUI_BUILD_EXAMPLE OFF CACHE BOOL " " FORCE)
set(NANOGUI_BUILD_PYTHON  OFF CACHE BOOL " " FORCE)
set(NANOGUI_INSTALL       OFF CACHE BOOL " " FORCE)
set(NANOGUI_USE_GLAD_DEFAULT ON CACHE BOOL " " FORCE)
add_subdirectory(ext/nanogui)
add_subdirectory(ext/clipper/cpp)
include_directories(${NANOGUI_EXTRA_INCS})
include_directories(ext/nanogui/include)
########### NanoGUI ################

include_directories(ext/clipper/cpp)

# Copy icons for an application
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/icons DESTINATION ${FM_BINARY_DIR})

# Application
add_subdirectory(app)
