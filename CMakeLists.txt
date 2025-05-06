cmake_minimum_required(VERSION 3.10)
project(graph_algorithms)

set (CMAKE_CXX_STANDARD 17)

SET(CMAKE_CXX_FLAGS "-O0")
SET(CMAKE_C_FLAGS "-O0")

add_library(graph_algorithms_lib src/adjacency_list_graph.cpp src/adjacency_matrix_graph.cpp)
target_include_directories(graph_algorithms_lib PUBLIC include/)

add_executable(test_sp
        src/sp_test_graphs.cpp src/shortest_path_algorithms.cpp)
target_link_libraries(test_sp graph_algorithms_lib)
target_compile_definitions(test_sp PUBLIC DATA_DIR_PATH="${CMAKE_CURRENT_SOURCE_DIR}/sp_data/")

add_executable(test_mst
        src/mst_test_graphs.cpp src/minimum_spanning_tree_algorithms.cpp)
target_link_libraries(test_mst graph_algorithms_lib)
target_compile_definitions(test_mst PUBLIC DATA_DIR_PATH="${CMAKE_CURRENT_SOURCE_DIR}/mst_data/")

add_executable(efficiency_tests
        src/main.cpp)
target_link_libraries(efficiency_tests graph_algorithms_lib)
