cmake_minimum_required(VERSION 3.5)

project(demo10)


aux_source_directory(. SRC_LIST)

# create static lib
add_library(demo10_static STATIC ${SRC_LIST})

# create dynamic lib
add_library(demo10 SHARED ${SRC_LIST})


install(TARGETS demo10_static
        DESTINATION bin
        COMPONENT static)

install(FILES demo10.h DESTINATION include COMPONENT static)

install(TARGETS demo10
        DESTINATION bin
        COMPONENT shared)

install(FILES demo10.h DESTINATION include COMPONENT shared)

add_custom_target(demo10_static_install
        DEPENDS demo10_static
        COMMAND
            "${CMAKE_COMMAND}" -DCMAKE_INSTALL_COMPONENT=static
            -P "${CMAKE_BINARY_DIR}/cmake_install.cmake"
)

add_custom_target(demo10_install
        DEPENDS demo10
        COMMAND
            "${CMAKE_COMMAND}" -DCMAKE_INSTALL_COMPONENT=shared
            -P "${CMAKE_BINARY_DIR}/cmake_install.cmake"
)
