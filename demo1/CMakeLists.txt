cmake_minimum_required(VERSION 3.5)
project(demo1)
set(CMAKE_CXX_STANDARD 11)

# below echo defalut cmake build type
#message("default build type is ${CMAKE_BUILD_TYPE}")
# you can set cmake build type to DEBUG
#set(CMAKE_BUILD_TYPE DEBUG)
#message("after set, build type is ${CMAKE_BUILD_TYPE}")
message(STATUS "binary dir is " ${CMAKE_BINARY_DIR})
message(STATUS "src dir is " ${CMAKE_SOURCE_DIR})

set(SOURCE_FILES main.cpp ../demo8/demo8.cpp)
add_executable(demo1 ${SOURCE_FILES})
