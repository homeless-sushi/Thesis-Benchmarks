# Set CMake Version
cmake_minimum_required(VERSION 3.7...3.25)

# Set CMake Policy (automatically set to version if VERSION >= 3.12)
if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

#Project name
project(
    BfsProject
    LANGUAGES CUDA CXX)

#Enable debug with cuda-gdb
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -g -G") 
endif()

#Enable OpenMP on NVCC
#set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -Xcompiler -fopenmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

#When the project is built, create these executables
add_executable(BFSAlgorithm
    "src/bfs.cc"
)

target_include_directories(BFSAlgorithm PUBLIC "include")

#Run the CMake of libs/HMLSS_Graph, then add the generated library
add_subdirectory("libs/HMLSS_Graph")
target_link_libraries(BFSAlgorithm PUBLIC HMLSS_Graph_LIB)

#Run the CMake of libs/HMLSS_BFS, then add the generated library
add_subdirectory("libs/HMLSS_Bfs")
target_link_libraries(BFSAlgorithm PUBLIC HMLSS_Bfs_LIB)

#Run the CMake of libs/utils, then add the generated library
add_subdirectory("libs/utils")
target_link_libraries(BFSAlgorithm PUBLIC utils_LIB)

#Boost Program Options
find_package(Boost REQUIRED
    COMPONENTS program_options)
target_include_directories(BFSAlgorithm PUBLIC ${Boost_INCLUDE_DIR})
target_link_libraries(BFSAlgorithm PUBLIC ${Boost_LIBRARIES})


if(BUILD_IS STREQUAL BENCHMARK)

    add_executable(BFSBenchmark
        "src/benchmark.cc"
    )
    target_include_directories(BFSBenchmark 
        PUBLIC 
            "include"
            ${Boost_INCLUDE_DIR}
    )
    target_link_libraries(BFSBenchmark 
        PUBLIC 
            HMLSS_Graph_LIB
            HMLSS_Bfs_LIB
            ${Boost_LIBRARIES}
    )

    find_package (margot_heel_generator REQUIRED)
    get_filename_component(MARGOT_JSON "margot/margot_benchmark.json" ABSOLUTE)
    get_filename_component(OPS_JSON "margot/ops.json" ABSOLUTE)
    margot_heel_generate_interface("${MARGOT_JSON}" "${OPS_JSON}")
    target_link_libraries(BFSBenchmark PUBLIC margot::margot_heel_interface ${CUDA_LIBRARIES})
    target_compile_features(BFSBenchmark PRIVATE cxx_std_17 )

    #TODO: Use find_package(); for this, the communication libraries have to be setup 
    #      with a cmake configuration file.
    #
    set(RtrmCommunication_DIR "/home/vivian/Documents/Thesis/Thesis-Communication")
    add_subdirectory(${RtrmCommunication_DIR} "${RtrmCommunication_DIR}/build")
    target_link_libraries(BFSBenchmark PUBLIC AppRegister_Common_Lib)
    target_link_libraries(BFSBenchmark PUBLIC AppRegister_Client_Lib)

elseif(BUILD_IS STREQUAL PROFILING)

    add_executable(BFSProfiling
        "src/profiling.cc"
    )
    target_include_directories(BFSProfiling 
        PUBLIC 
            "include"
            ${Boost_INCLUDE_DIR}
    )
    target_link_libraries(BFSProfiling 
        PUBLIC 
            HMLSS_Graph_LIB
            HMLSS_Bfs_LIB
            ${Boost_LIBRARIES}
    )

    find_package (margot_heel_generator REQUIRED)
    get_filename_component(MARGOT_JSON "margot/margot_profiling.json" ABSOLUTE)
    margot_heel_generate_interface("${MARGOT_JSON}")
    target_link_libraries(BFSProfiling PUBLIC margot::margot_heel_interface ${CUDA_LIBRARIES})
    target_compile_features(BFSProfiling PRIVATE cxx_std_17 )

endif()
