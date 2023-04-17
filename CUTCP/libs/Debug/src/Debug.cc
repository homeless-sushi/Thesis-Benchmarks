#include <Debug/Debug.h>

#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include <Atom/Atom.h>

namespace Debug
{
    void WriteAtoms(
        const std::string fileURL,
        const std::vector<Atom::Atom>& atoms)
    {
	    std::ofstream outFile(fileURL);
        if (!outFile.is_open()){
            throw std::runtime_error("Cannot open file: " + fileURL);
        }

        outFile << std::fixed << std::setprecision(6);
        for (const auto& atom : atoms){
            outFile << atom.x << " " << atom.y << " " << atom.z << " " << atom.q << std::endl;
        }
    }

    void WriteLattice(
        const std::string fileURL,
        const Lattice::Lattice& lattice)
    {
	    std::ofstream outFile(fileURL);
        if (!outFile.is_open()){
            throw std::runtime_error("Cannot open file: " + fileURL);
        }
        
        outFile << std::fixed << std::setprecision(6);
        for (const auto& potential : lattice.points){
            outFile << potential << std::endl;
        }
    }
}