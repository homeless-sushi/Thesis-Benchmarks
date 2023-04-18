#ifndef CUTCP_CUTCP_CPU_H
#define CUTCP_CUTCP_CPU_H

#include <vector>

#include <Atom/Atom.h>
#include <Atom/Lattice.h>

namespace Cutcp
{
    void CutoffPotential(
        Lattice::Lattice& lattice, 
        float cutoff, 
        std::vector<Atom::Atom>& atoms
    );
    void RemoveExclusions(
        Lattice::Lattice& lattice, 
        float exclusionCutoff, 
        std::vector<Atom::Atom>& atoms
    );
}

#endif //CUTCP_CUTCP_CPU_H