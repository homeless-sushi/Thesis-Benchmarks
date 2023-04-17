# Set CMake Version
cmake_minimum_required(VERSION 3.7...3.25)

# Set CMake Policy (automatically set to version if VERSION >= 3.12)
if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

#Project name
project(
    CutcpProject
    LANGUAGES CUDA CXX)

#Enable debug with cuda-gdb
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -g -G") 
endif()

#Enable OpenMP on NVCC
#set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -Xcompiler -fopenmp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

#When the project is built, create this executables
add_executable(CutcpAlgorithm)

#Add sources to the target
target_sources(CutcpAlgorithm
    PRIVATE
        "src/cutcp.cc"
)

#Add to the include path of the target
target_include_directories(CutcpAlgorithm 
    PUBLIC 
        "include"
)

#Run the CMake of this subdirectory, then add the generated library as dependency
add_subdirectory("libs/Atom")
add_subdirectory("libs/Cutcp")
add_subdirectory("libs/Debug")
target_link_libraries(CutcpAlgorithm
    PUBLIC
        Atom_LIB
        Cutcp_LIB
        Debug_LIB
)

#Boost Program Options
find_package(Boost REQUIRED
    COMPONENTS program_options)
target_include_directories(CutcpAlgorithm PUBLIC ${Boost_INCLUDE_DIR})
target_link_libraries(CutcpAlgorithm PUBLIC ${Boost_LIBRARIES})

