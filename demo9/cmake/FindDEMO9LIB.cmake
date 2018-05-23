message("now using FindDEMO9LIB.cmake find demo9 lib")

FIND_PATH(DEMO9LIB_INCLUDE_DIR demo9.h /usr/include/demo9/
        /usr/local/demo9/include/)
message("./h dir ${DEMO9LIB_INCLUDE_DIR}")

FIND_LIBRARY(DEMO9LIB_LIBRARY libdemo9_lib.a /usr/local/demo9/lib/)
message("lib dir: ${DEMO9LIB_LIBRARY}")

if(DEMO9LIB_INCLUDE_DIR AND DEMO9LIB_LIBRARY)
    set(DEMO9LIB_FOUND TRUE)
endif(DEMO9LIB_INCLUDE_DIR AND DEMO9LIB_LIBRARY)
