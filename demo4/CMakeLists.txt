cmake_minimum_required(VERSION 3.5)

project(demo4)

# This project show multi files build in cmake.
include_directories(demo4_inc)

aux_source_directory(. SRC_LIST)
message("src list is ${SRC_LIST}")
# if not add src in demo4_inc, then you will get "undefined reference to `demo4::print_demo4()"
aux_source_directory(demo4_inc SRC_LIST)
message("after add demo4_inc directory src list is ${SRC_LIST}")

add_executable(demo4 ${SRC_LIST})