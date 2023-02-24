#When the project is built, create this library
add_library(HMLSS_Bfs_LIB STATIC
    "src/BfsKnobs.cc"
    "src/Bfs.cc"
    "src/BfsUtils.cc"
    "src/BfsCpu.cc"
    "src/BfsCuda.cu")

#Add to the include path
target_include_directories(HMLSS_Bfs_LIB PUBLIC "include")

#Add HMLSS Graph dependency
target_link_libraries(HMLSS_Bfs_LIB HMLSS_Graph_LIB)

#Add utils dependency
target_link_libraries(HMLSS_Bfs_LIB utils_LIB)