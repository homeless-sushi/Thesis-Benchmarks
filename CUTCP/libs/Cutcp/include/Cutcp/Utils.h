#ifndef CUTCP_UTILS_H
#define CUTCP_UTILS_H

#include <string>

#include <Atom/Lattice.h>

namespace Cutcp
{
    void WriteLatticeFile(
        std::string fileURL,
        Lattice::Lattice& lattice);
}

#endif //CUTCP_UTILS_H