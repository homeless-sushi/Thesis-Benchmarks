#When the project is built, create this library
add_library(HMLSS_Graph_LIB STATIC 
    "src/Graph.cc"
    "src/GraphUtils.cc")

#Add to the include path
target_include_directories(HMLSS_Graph_LIB PUBLIC "include")