project(proto_lib)
cmake_minimum_required(VERSION 3.5)

SET(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")

find_package(Protobuf REQUIRED)

add_subdirectory(my_proto)

message("main Protobuf lib: ${PROTOBUF_LIBRARIES}, inc: ${PROTOBUF_INCLUDE_DIR}")

add_executable(proto_lib_demo proto_lib_demo.cpp)
target_link_libraries(proto_lib_demo
        proto_lib
        )

get_target_property(INCLUDES proto_lib_demo INCLUDE_DIRECTORIES)
message(STATUS "proto_lib_demo include directories: ${INCLUDES}")

get_target_property(LINK_LIBS proto_lib_demo LINK_LIBRARIES)
message(STATUS "proto_lib_demo link libraries: ${LINK_LIBS}")

