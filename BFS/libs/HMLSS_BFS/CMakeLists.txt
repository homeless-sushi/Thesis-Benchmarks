#When the project is built, create this library
add_library(HMLSS_BFS STATIC
    "src/BFS.cc"
    "src/BFSKnobs.cc"
    "src/BFSUtils.cc"
    "src/BFS_OMP.cc")

#Add to the include path
target_include_directories(HMLSS_BFS PUBLIC "include")
#target_include_directories(HMLSS_BFS PUBLIC "${PROJECT_SOURCE_DIR}/libs/HMLSS_GRAPH")

#Add HMLSS Graph dependency
target_link_libraries(HMLSS_BFS HMLSS_Graph)