#When the project is built, create this library
add_library(HMLSS_Graph STATIC 
    "src/Graph.cc"
    "src/GraphUtils.cc")

#Add to the include path
target_include_directories(HMLSS_Graph PUBLIC "include")