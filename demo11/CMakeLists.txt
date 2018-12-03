cmake_minimum_required(VERSION 3.5)

project(demo11)

# The version number
set(demo11_VERSION_MAJOR 1)
set(demo11_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings to
# source code(demo11Config.h.in), the file demoConfig.h does't exists
configure_file(
        "${PROJECT_SOURCE_DIR}/demo11Config.h.in"
        "${PROJECT_BINARY_DIR}/demoConfig.h"
)

# add the binary tree to the search path for include files
# so that we will find demoConfig.h
include_directories("${PROJECT_BINARY_DIR}")

# add the exectuable
add_executable(demo11 demo11.cpp)
target_link_libraries(demo11
        demo7_1
        )
message("demo7_1 lib path: ${demo7_1}")