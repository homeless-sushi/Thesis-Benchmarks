#ifndef ATOM_UTILS_H
#define ATOM_UTILS_H

#include "Atom/Atom.h"

#include <vector>
#include <string>

namespace Atom
{
    std::vector<Atom> ReadAtomFile(std::string fileURL);
    void GetAtomBounds(const std::vector<Atom>& atoms, Vec3& min, Vec3& max);
}

#endif //ATOM_UTILS_H