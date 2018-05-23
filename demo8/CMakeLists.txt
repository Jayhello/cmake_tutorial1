cmake_minimum_required(VERSION 3.5)

project(demo8)

find_package(CURL)

aux_source_directory(./ SRC_LIST)
add_executable(demo8 ${SRC_LIST})

if(CURL_FOUND)
    message("curl include path: ${CURL_INCLUDE_DIR}, lib path ${CURL_LIBRARY}")
    include_directories(${CURL_INCLUDE_DIR})
    target_link_libraries(demo8 ${CURL_LIBRARY})
endif()

