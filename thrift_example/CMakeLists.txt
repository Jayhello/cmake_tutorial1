
add_compile_options(-std=c++11)
add_compile_options(-pthread)

FIND_LIBRARY(THRIFT_LIBRARY thrift)
FIND_LIBRARY(THRIFT_CPP2_LIBRARY thriftcpp2)
FIND_PATH(THRIFT_INCLUDE_DIR "thrift/lib/cpp2/Thrift.h")

SET(THRIFT_LIBRARIES ${THRIFT_LIBRARY} ${THRIFT_CPP2_LIBRARY})

message("found thrift head:${THRIFT_INCLUDE_DIR}, lib: ${THRIFT_LIBRARIES}")


# set thrift gen source code
set(thrift_src
    gen-cpp/search_id_types.cpp
    gen-cpp/search_id_types.h
    gen-cpp/Search.h
    gen-cpp/Search.cpp
    gen-cpp/search_id_constants.h
    gen-cpp/search_id_constants.cpp
#    gen-cpp/Search_server.skeleton.cpp
)

# create thrift lib
add_library(thrift_net ${thrift_src})
target_link_libraries(thrift_net ${THRIFT_LIBRARIES})

# create thrift client exe
add_executable(thrift_client thrift_client.cpp)
#target_link_libraries(thrift_client ${thrift_net})
target_link_libraries(thrift_client thrift_net)


# create thrift client exe
add_executable(thrift_server thrift_server.cpp)
target_link_libraries(thrift_server thrift_net)
