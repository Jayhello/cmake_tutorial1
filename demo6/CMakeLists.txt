cmake_minimum_required(VERSION 3.5)

project(demo6)

# To include demo5_h.h
#include_directories(/home/xy/cmake_practice/cmake_build/build_demo5/include/)

# to find libdemo5.so
link_directories(/home/xy/cmake_practice/cmake_build/build_demo5/lib/)

aux_source_directory(. SRC_LIST)

add_executable(demo6 ${SRC_LIST})
target_include_directories(demo6 PUBLIC /home/xy/cmake_practice/cmake_build/build_demo5/include/)

# link lib libdemo5.so
target_link_libraries(demo6 demo5)











