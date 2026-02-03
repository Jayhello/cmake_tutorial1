cmake_minimum_required(VERSION 3.5)

project(use_boost)

# compile options
add_compile_options(-std=c++11)

# ------------first way-------------------
set(Boost_NO_SYSTEM_PATHS FALSE)
if (Boost_NO_SYSTEM_PATHS)
    set(BOOST_ROOT "/home/xy/boost_install/lib")
    set(BOOST_INCLUDE_DIRS "${BOOST_ROOT}/include")
    set(BOOST_LIBRARY_DIRS "${BOOST_ROOT}/lib")
endif (Boost_NO_SYSTEM_PATHS)

find_package(Boost REQUIRED regex date_time system filesystem program_options)
include_directories(${BOOST_INCLUDE_DIRS})
message("boost lib: ${Boost_LIBRARIES}, inc:${Boost_INCLUDE_DIR}")
# ------------first way end-------------------


# ------------second way-------------------
#set( BOOST_ROOT "/home/xy/boost_install/lib/" CACHE PATH "Boost library path" )
#set( Boost_NO_SYSTEM_PATHS on CACHE BOOL "Do not search system for Boost" )
#
#find_package(Boost REQUIRED regex date_time system filesystem program_options)
#include_directories(${BOOST_INCLUDE_DIRS})
#message("boost lib: ${Boost_LIBRARIES}, inc:${Boost_INCLUDE_DIR}")

# ------------second way end---------------

# ------------third way using system boost-------------------
find_package(Boost REQUIRED regex date_time system filesystem program_options)
include_directories(${BOOST_INCLUDE_DIRS})
message("boost lib: ${Boost_LIBRARIES}")
message("boost inc:${Boost_INCLUDE_DIR}")
# ------------third way end-------------------

add_executable(use_boost use_boost.cpp)
target_link_libraries(use_boost
        ${Boost_LIBRARIES}
        )