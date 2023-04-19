#include <Cutcp/Cutcp.h>

#include <vector>

#include <Atom/Atom.h>
#include <Atom/Lattice.h>
#include <Atom/Utils.h>

namespace Cutcp
{ 
    Cutcp::Cutcp(std::vector<Atom::Atom>& atoms, float spacing) :
        atoms{atoms}
    {
        Atom::Vec3 minCoords;
        Atom::Vec3 maxCoords;
        Atom::GetAtomBounds(atoms, minCoords, maxCoords);

        lattice_ = Lattice::Lattice(minCoords, maxCoords, spacing);
    };

    Cutcp::~Cutcp() = default;
}
