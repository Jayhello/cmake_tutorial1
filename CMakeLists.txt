cmake_minimum_required(VERSION 3.5)

# The project demo1 shows hwo to write a sample cmake project,
# and shows how to build debug and release project
add_subdirectory(demo1)

# The project demo2 shows
add_subdirectory(demo2)
add_subdirectory(demo3)
add_subdirectory(demo4)

# The project demo5 shows how to create .a lib and .so lib
# and install it
add_subdirectory(demo5)

# link library and include directory
add_subdirectory(demo6)

# use macro to create multi libs
add_subdirectory(demo7)

# use find package
add_subdirectory(demo8)

add_subdirectory(demo9)

# This demo shows use add_custom_target and install COMPONENT
# to set up custom targets for the installations
# Mainly cited from: stack overflow
add_subdirectory(demo10)
