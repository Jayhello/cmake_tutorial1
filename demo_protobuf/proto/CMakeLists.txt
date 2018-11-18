
protobuf_generate_cpp(PROTO_SRC PROTO_HEADER message.proto)
add_library(proto
        ${PROTO_HEADER}
        ${PROTO_SRC}
        )
