#When the project is built, create this library
add_library(Atom_LIB)

#Add executable
target_sources(Atom_LIB
    PRIVATE
        "src/Utils.cc"
)

#Add to the include path
target_include_directories(Atom_LIB 
    PUBLIC
        "include"
)