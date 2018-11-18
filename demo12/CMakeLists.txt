cmake_minimum_required(VERSION 3.5)

# should we use specified function
# if not set, then will use common function
option(USE_SPECIFIED "select to use specified function" ON)

# select to use specified function, or common function
message("USE_SPECIFIED is ${USE_SPECIFIED}")

if(USE_SPECIFIED)
    add_definitions(-DUSE_SPECIFIED)
    include_directories("${PROJECT_SOURCE_DIR}/SpecifiedFun")
    add_subdirectory(SpecifiedFun)
    set(EXTRA_LIBS ${EXTRA_LIBS} specified_fun)
    message("select to use specified function, lib is: ${EXTRA_LIBS}")
else()
    include_directories("${PROJECT_SOURCE_DIR}/CommonFun")
    add_subdirectory(CommonFun)
    set(EXTRA_LIBS ${EXTRA_LIBS} common_fun)
    message("select to use common function, lib is: ${EXTRA_LIBS}")
endif(USE_SPECIFIED)

add_executable(demo12 demo12.cpp)
target_link_libraries(demo12 ${EXTRA_LIBS})

