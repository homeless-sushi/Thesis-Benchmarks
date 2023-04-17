#ifndef ATOM_LATTICE_H
#define ATOM_LATTICE_H

#include <Atom/Atom.h>

#include <vector>

#include <cmath>

namespace Lattice
{
    /* 
     * A class that maps points in the 3D lattice to their coordinates
     */
    class LatticeDim 
    {
        private:
            int nx_; // Number of lattice points in the x dimension
            int ny_; // Number of lattice points in the y dimension
            int nz_; // Number of lattice points in the z dimension
            Atom::Vec3 min_; // Lowest corner of lattice
            float spacing_; // Spacing between potential points

        public:
            LatticeDim(Atom::Vec3 minCoords, Atom::Vec3 maxCoords, float spacing) :
                nx_{(int) floor((maxCoords.x-minCoords.x)/spacing) + 1},
                ny_{(int) floor((maxCoords.y-minCoords.y)/spacing) + 1},
                nz_{(int) floor((maxCoords.z-minCoords.z)/spacing) + 1},
                min_{minCoords},
                spacing_{spacing}
            {};

            Atom::Vec3 getPointCoordinates(int x, int y, int z);
            int nx() const { return nx_; }
            int ny() const { return nz_; }
            int nz() const { return ny_; }
            const Atom::Vec3& min() const { return min_; }
            float spacing() const { return spacing_; }
    };

    class Lattice
    {
        public:
            LatticeDim dim;
            std::vector<float> points;

            Lattice(LatticeDim dim) :
                dim{dim},
                points(((dim.nx() * dim.ny() * dim.nz()) + 7) & ~7, 0) // Round to multiple of 8
            {};
    };
}

#endif //ATOM_LATTICE_H
