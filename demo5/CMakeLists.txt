cmake_minimum_required(VERSION 3.5)

project(demo5)

#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(DEMO5_VERSION_MAJOR 1)
set(DEMO5_VERSION_MINOR 1)
set(DEMO5_VERSION_PATCH 2)
set(DEMO5_VERSION ${DEMO5_VERSION_MAJOR}.${DEMO5_VERSION_MINOR}.${DEMO5_VERSION_PATCH})

aux_source_directory(. SRC_LIST)

# create static lib
#add_library(demo5 STATIC ${SRC_LIST})

# create dynamic lib
#add_library(demo5_dynamic SHARED ${SRC_LIST})
add_library(demo5 SHARED ${SRC_LIST})

# install lib and .h
install(TARGETS demo5 DESTINATION lib)
install(FILES demo5_h.h DESTINATION include)

set_target_properties(demo5 PROPERTIES VERSION ${DEMO5_VERSION}
        SOVERSION ${DEMO5_VERSION_MAJOR}
        )


