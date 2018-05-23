cmake_minimum_required(VERSION 3.5)

project(demo5)

macro(add_lib file_name)
    add_library(${file_name} STATIC ${file_name}.cpp)
endmacro()

add_lib(demo7_1)
add_lib(demo7_2)


