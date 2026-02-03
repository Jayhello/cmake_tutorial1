# proto/CMakeLists.txt
# 方法1：最简单的写法
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS
        meta.proto
        common.proto
        )

add_library(proto_lib ${PROTO_SRCS} ${PROTO_HDRS})

# 关键：使用 PUBLIC 修饰符
target_include_directories(proto_lib PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}    # 这个路径会被传递给链接者
        ${Protobuf_INCLUDE_DIRS}
        )

message("inc: ${CMAKE_CURRENT_BINARY_DIR}")

target_link_libraries(proto_lib PUBLIC ${Protobuf_LIBRARIES})  # PUBLIC 很重要！
