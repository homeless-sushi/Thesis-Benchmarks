#ifndef DEBUG_DEBUG_H
#define DEBUG_DEBUG_H

#include <string>
#include <vector>

#include <Atom/Atom.h>
#include <Atom/Lattice.h>

namespace Debug
{
    void WriteAtoms(
        const std::string fileURL,
        const std::vector<Atom::Atom>& atoms);

    void WriteLattice(
        const std::string fileURL,
        const Lattice::Lattice& lattice);
}

#endif //DEBUG_DEBUG_H