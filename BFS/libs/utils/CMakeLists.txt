add_library(utils_LIB INTERFACE)

target_include_directories(utils_LIB INTERFACE "include")

#Add Boost (all header only libraries)
find_package(Boost 1.60.0 REQUIRED)
target_include_directories(utils_LIB INTERFACE ${Boost_INCLUDE_DIR})