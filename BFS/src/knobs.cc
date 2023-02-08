#include "knobs.h"

namespace Knobs 
{
    CPUKnobs::CPUKnobs() : nThreads(1) {}
    CPUKnobs::CPUKnobs(unsigned int nThreads) : nThreads(nThreads) {}

    Knobs::Knobs() : device(DEVICE::CPU), cpuKnobs() {}
}
