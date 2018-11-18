project(proto_demo)
cmake_minimum_required (VERSION 3.5)
SET(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")

find_package(Protobuf REQUIRED)

add_subdirectory(proto)

message("main Protobuf lib: ${PROTOBUF_LIBRARIES}, inc: ${PROTOBUF_INCLUDE_DIR}")

# for proto/message.pb.h
include_directories(${CMAKE_CURRENT_BINARY_DIR})
message("lib: ${CMAKE_CURRENT_BINARY_DIR}")

add_executable(proto_demo main.cpp)
target_link_libraries(proto_demo
        proto
        ${PROTOBUF_LIBRARY}
        )

#SET(PROTO_META_BASE_DIR ${CMAKE_CURRENT_BINARY_DIR})
#LIST(APPEND PROTO_FLAGS -I${CMAKE_CURRENT_SOURCE_DIR})
#
#FILE(GLOB BAR_PROTOS "*.proto")
#
#FOREACH(FIL ${BAR_PROTOS})
#    GET_FILENAME_COMPONENT(ABS_FIL ${FIL} ABSOLUTE)
#    GET_FILENAME_COMPONENT(FIL_WE ${FIL} NAME_WE)
#
#    LIST(APPEND BAR_SRCS "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.cc")
#    LIST(APPEND BAR_HDRS "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.h")
#
#    EXECUTE_PROCESS(
#            COMMAND ${PROTOBUF_PROTOC_EXECUTABLE} ${PROTO_FLAGS} --cpp_out=${PROTO_META_BASE_DIR} ${FIL}
#            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#    )
#ENDFOREACH()
#
#ADD_LIBRARY(bar ${BAR_SRCS})
#TARGET_LINK_LIBRARIES(bar foo)
