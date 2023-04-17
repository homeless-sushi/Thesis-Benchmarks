#include "Atom/Atom.h"
#include "Atom/Utils.h"

#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <cmath>

namespace Atom
{
    std::vector<Atom> ReadAtomFile(std::string fileURL)
    {
        std::ifstream atomFile(fileURL);
        if (!atomFile.is_open()){
            throw std::runtime_error("Cannot open file: " + fileURL);
        }

        std::string lineString;
        std::vector<Atom> atoms;
        while(atomFile.good()){

            getline(atomFile, lineString);
            std::istringstream lineStream(lineString);
            std::string lineType;
            lineStream >> lineType; 
            if(lineType == "ATOM"){
                Atom atom;
                
                //Ignore 4 fields, with uknown function
                {
                    std::string tmp;
                    for(size_t i = 0; i < 4; ++i) 
                        lineStream >> tmp;
                }

                lineStream >> atom.x;
                lineStream >> atom.y;
                lineStream >> atom.z;
                lineStream >> atom.q;

                atoms.push_back(atom);
            }
        }

        return atoms;
    }

    void GetAtomBounds(const std::vector<Atom>& atoms, Vec3& min, Vec3& max)
    {
        if(!atoms.size()){
            throw std::runtime_error("Empty atoms vector");
        }
        
        max.x = min.x = atoms.front().x;
        max.y = min.y = atoms.front().y;
        max.z = min.z = atoms.front().z;

        for(const auto atom : atoms) {
            min.x = fmin(min.x, atom.x);
            max.x = fmax(max.x, atom.x);
            min.y = fmin(min.y, atom.y);
            max.y = fmax(max.y, atom.y);
            min.z = fmin(min.z, atom.z);
            max.z = fmax(max.z, atom.z);
        }
    }
}
