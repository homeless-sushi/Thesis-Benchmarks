#When the project is built, create this library
add_library(Debug_LIB)

#Add executable
target_sources(Debug_LIB
    PRIVATE
        "src/Debug.cc"
)

#Add to the include path
target_include_directories(Debug_LIB 
    PUBLIC
        "include"
)

#Add Atom dependency
target_link_libraries(Debug_LIB Atom_LIB)
target_link_libraries(Debug_LIB Cutcp_LIB)