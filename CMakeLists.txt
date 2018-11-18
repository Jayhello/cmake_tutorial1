cmake_minimum_required(VERSION 3.5)

# The project demo1 shows hwo to write a sample cmake project,
# and shows how to build debug and release project
add_compile_options(-std=c++11)

set(LIB_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/lib")

set(BIN_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin")

message("LIB_OUTPUT_PATH ${LIB_OUTPUT_PATH}")
message("BIN_OUTPUT_PATH ${BIN_OUTPUT_PATH}")

add_subdirectory(demo1)

# The project demo2 shows
add_subdirectory(demo2)
add_subdirectory(demo3)
add_subdirectory(demo4)

# The project demo5 shows how to create .a lib and .so lib
# and install it
add_subdirectory(demo5)

# link library and include directory
add_subdirectory(demo6)

# use macro to create multi libs
add_subdirectory(demo7)

# use find package
add_subdirectory(demo8)

add_subdirectory(demo9)

# This demo shows use add_custom_target and install COMPONENT
# to set up custom targets for the installations
# Mainly cited from: stack overflow
add_subdirectory(demo10)

add_subdirectory(demo11)

add_subdirectory(demo12)

add_subdirectory(demo_knn)

# protobuf demo
add_subdirectory(demo_protobuf)

# a more hard demo
add_subdirectory(demo_hard1)

# use local boost
add_subdirectory(demo_local_boost)

# usage of boost asio
add_subdirectory(demo_boost_asio)

# s2s demo
add_subdirectory(s2s_example)

# thrift example
add_subdirectory(thrift_example)