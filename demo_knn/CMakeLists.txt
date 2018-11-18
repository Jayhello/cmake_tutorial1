project(cv_knn_demo)

add_compile_options(-std=c++11)

include_directories(~/opencv3.1_install/lib/include/)
link_directories(~/opencv3.1_install/lib/lib/)

add_executable(cv_knn_demo knn_kdtree_demo.cpp)

message("demo_knn ===================== BIN_OUTPUT_PATH ${BIN_OUTPUT_PATH}")

set_target_properties(cv_knn_demo
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${BIN_OUTPUT_PATH}"
        )

target_link_libraries(cv_knn_demo
        -lopencv_ml
        -lopencv_core
        -lopencv_highgui
        -lopencv_imgcodecs)
