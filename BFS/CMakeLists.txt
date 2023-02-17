# Set CMake Version
cmake_minimum_required(VERSION 3.7...3.25)

# Set CMake Policy (automatically set to version if VERSION >= 3.12)
if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

#Project name
project(
    BFSApp
    LANGUAGES CUDA CXX)

#Enable debug with cuda-gdb
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -g -G") 
endif()

#Enable OpenMP on NVCC
#set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -Xcompiler -fopenmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

#When the project is built, create this library
set(APP_SOURCES "src/main.cc")
add_executable(BFSApp ${APP_SOURCES})

target_include_directories(BFSApp PUBLIC "include")

#Run the CMake of libs/HMLSS_Graph, then add the generated library
set(GRAPH_LIB "HMLSS_Graph")
add_subdirectory("libs/${GRAPH_LIB}")
target_link_libraries(BFSApp PUBLIC HMLSS_Graph)

#Run the CMake of libs/HMLSS_BFS, then add the generated library
set(BFS_LIB "HMLSS_BFS")
add_subdirectory("libs/${BFS_LIB}")
target_link_libraries(BFSApp PUBLIC HMLSS_BFS)

#Margot Heel
find_package (margot_heel_generator REQUIRED)
get_filename_component(MARGOT_JSON "margot/margot.json" ABSOLUTE)
margot_heel_generate_interface("${MARGOT_JSON}")
target_link_libraries(BFSApp PUBLIC margot::margot_heel_interface ${CUDA_LIBRARIES})
target_compile_features(BFSApp PRIVATE cxx_std_17 )
