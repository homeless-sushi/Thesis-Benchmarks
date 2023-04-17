#include <Atom/Atom.h>
#include <Atom/Lattice.h>

#include <vector>

namespace Lattice
{
    Atom::Vec3 LatticeDim::getPointCoordinates(int x, int y, int z)
    {
        return min_ + Atom::Vec3(x,y,z)*spacing_;
    };
}
