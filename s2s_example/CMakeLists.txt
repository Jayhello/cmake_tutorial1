project(s2s_example)

add_compile_options(-std=c++0x)

include_directories(/home/xy/s2s_code/s2ssdk-master-622dd325e1126d1d1a8f6eb8196dc75c4e271108/libev-4.11/)
link_directories(/home/xy/cmake_practice/cmake_tuorial/lib/)

#add_definitions(GLIBCXX_USE_CXX11_ABI=1)
#add_definitions(_GLIBCXX_USE_CXX11_ABI)


add_executable(s2s_demo1 demo.cpp)
target_link_libraries(s2s_demo1
        /home/xy/cmake_practice/cmake_tuorial/lib/libs2sclient.so.3
        /lib/x86_64-linux-gnu/libuuid.so.1.3.0
        /usr/lib/x86_64-linux-gnu/libcrypto.a
        pthread
        )

#find_library(LIBEVENT2_CORE event_core)
#message("libev: ${LIBEVENT2_CORE}")

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/)
find_package(Libevent)
message("libev: ${LIBEVENT_LIBRARIES}, ${LIBEVENT_INCLUDE_DIRS}")


add_executable(s2s_demo2 demo1.cpp)
target_link_libraries(s2s_demo2
        /home/xy/cmake_practice/cmake_tuorial/lib/libs2sclient.so.3
        /lib/x86_64-linux-gnu/libuuid.so.1.3.0
        /usr/lib/x86_64-linux-gnu/libcrypto.a
        /usr/lib/x86_64-linux-gnu/libevent.so
#        ${LIBEVENT2_CORE}
        ${LIBEVENT_LIBRARIES}
        /usr/lib/x86_64-linux-gnu/libev.so.4
        pthread
        )

add_executable(s2s_demo3 demo2.cpp)
target_link_libraries(s2s_demo3
        /home/xy/cmake_practice/cmake_tuorial/lib/libs2sclient.so.3
        /lib/x86_64-linux-gnu/libuuid.so.1.3.0
        /usr/lib/x86_64-linux-gnu/libcrypto.a
        /usr/lib/x86_64-linux-gnu/libevent.so
        #        ${LIBEVENT2_CORE}
        ${LIBEVENT_LIBRARIES}
        /usr/lib/x86_64-linux-gnu/libev.so.4
        pthread
        )