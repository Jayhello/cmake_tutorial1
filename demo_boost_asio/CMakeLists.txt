
project(boost_asio_demo)

add_compile_options(-std=c++11)
add_compile_options(-pthread)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

find_package(Boost COMPONENTS system REQUIRED)

# include boost directory
include_directories(${Boost_INCLUDE_DIR})
message("find boost lib: ${Boost_LIBRARIES}")

set(CLIENT_SRC chat_client.cpp)
add_executable(boost_client ${CLIENT_SRC})
target_link_libraries(boost_client ${Boost_LIBRARIES})


set(SERVER_SRC chat_server.cpp)
add_executable(boost_server ${SERVER_SRC})
target_link_libraries(boost_server ${Boost_LIBRARIES})
