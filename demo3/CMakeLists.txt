cmake_minimum_required(VERSION 3.5)

project(demo3)

# This project show multi files build in cmake.

# first way: set source file by name
#set(SRC_LIST demo3_main.cpp demo_3.cpp)

# second way: use this get all the files in the current directory
# aux_source_directory(. SRC_LIST)

# third way: file glob
file(GLOB SRC_LIST ./*.cpp)

add_executable(demo3 ${SRC_LIST})