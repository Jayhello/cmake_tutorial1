cmake_minimum_required(VERSION 3.5)

project(demo_hard1 VERSION 1.1.7)

add_compile_options(-std=c++11)

include(CheckIncludeFile)
check_include_file("byteswap.h" HAVE_BYTESWAP_H)
check_include_file("stddef.h" HAVE_STDDEF_H)
check_include_file("stdint.h" HAVE_STDINT_H)
check_include_file("unistd.h" HAVE_UNISTD_H)
check_include_file("windows.h" HAVE_WINDOWS_H)

include(CheckLibraryExists)
check_library_exists(z zlibVersion "" HAVE_LIBZ)
check_library_exists(lzo2 lzo1x_1_15_compress "" HAVE_LIBLZO2)

configure_file(
        "${PROJECT_SOURCE_DIR}/cmake/config.h.in"
        "${PROJECT_BINARY_DIR}/config.h"
)

# We don't want to define HAVE_ macros in public headers. Instead, we use
# CMake's variable substitution with 0/1 variables, which will be seen by the
# preprocessor as constants.
set(HAVE_STDINT_H_01  ${HAVE_STDINT_H})
set(HAVE_STDDEF_H_01  ${HAVE_STDDEF_H})
set(HAVE_SYS_UIO_H_01 ${HAVE_SYS_UIO_H})

if(NOT HAVE_STDINT_H_01)
    set(HAVE_STDINT_H_01 0)
endif(NOT HAVE_STDINT_H_01)
if(NOT HAVE_STDDEF_H_01)
    set(HAVE_STDDEF_H_01 0)
endif(NOT HAVE_STDDEF_H_01)

configure_file(
        "${PROJECT_SOURCE_DIR}/test_public.h.in"
        "${PROJECT_BINARY_DIR}/test_public.h"
)

add_library(snappy "")
target_sources(snappy
        PRIVATE
        "${PROJECT_SOURCE_DIR}/test_hard1.cpp"
        "${PROJECT_SOURCE_DIR}/test_hard1.h"
        "${PROJECT_BINARY_DIR}/config.h"
)

target_include_directories(snappy
        PUBLIC
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
        )

set_target_properties(snappy
        PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR}
)

install(TARGETS snappy
        EXPORT SnappyTargets
        RUNTIME DESTINATION bin ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION lib ${CMAKE_INSTALL_LIBDIR}
        # ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR # DESTINATION error
        ARCHIVE DESTINATION lib ${CMAKE_INSTALL_LIBDIR}
)

install(
        FILES
        "${PROJECT_SOURCE_DIR}/test_hard1.h"
        "${PROJECT_BINARY_DIR}/config.h"
        DESTINATION include ${CMAKE_INSTALL_INCLUDEDIR}
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${PROJECT_BINARY_DIR}/SnappyConfigVersion.cmake"
        COMPATIBILITY SameMajorVersion
)

install(
        EXPORT SnappyTargets
        NAMESPACE Snappy::
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Snappy"
)

install(
        FILES
        "${PROJECT_SOURCE_DIR}/cmake/SnappyConfig.cmake"
        "${PROJECT_BINARY_DIR}/SnappyConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Snappy"
)