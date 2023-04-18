#When the project is built, create this library
add_library(Cutcp_LIB)

#Add executable
target_sources(Cutcp_LIB
    PRIVATE
        "src/CutcpCpu.cc"
        "src/Utils.cc"
)

#Add to the include path
target_include_directories(Cutcp_LIB 
    PUBLIC
        "include"
)

#Add Atom dependency
target_link_libraries(Cutcp_LIB Atom_LIB)