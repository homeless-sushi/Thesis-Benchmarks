#ifndef CUTCP_CUTCP_H
#define CUTCP_CUTCP_H

#include <vector>

#include <Atom/Atom.h>
#include <Atom/Lattice.h>

namespace Cutcp
{
    class Cutcp
    {
        protected:
            Lattice::Lattice lattice_;

        public:
            std::vector<Atom::Atom>& atoms;  

            Cutcp(std::vector<Atom::Atom>& atoms, float spacing);
            virtual ~Cutcp();
            virtual bool cutoffPotential() = 0; //first kernel
            virtual bool removeExclusions() = 0; //second kernel
            virtual const std::vector<int>& lattice() = 0;
    };
}

#endif //CUTCP_CUTCP_H