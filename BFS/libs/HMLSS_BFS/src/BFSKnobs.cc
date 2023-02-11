#include "HMLSS_BFS/BFSKnobs.h"

namespace BFSKnobs 
{
    CPUKnobs::CPUKnobs() : nThreads(1) {}
    CPUKnobs::CPUKnobs(unsigned int nThreads) : nThreads(nThreads) {}

    Knobs::Knobs() : device(DEVICE::CPU), cpuKnobs() {}
}
