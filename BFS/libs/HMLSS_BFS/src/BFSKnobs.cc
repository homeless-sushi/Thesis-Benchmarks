#include "HMLSS_BFS/BFSKnobs.h"

namespace BFSKnobs 
{
    CPUKnobs::CPUKnobs() : nThreads(1) {}
    CPUKnobs::CPUKnobs(unsigned int nThreads) : nThreads(nThreads) {}

    GPUKnobs::GPUKnobs(BLOCK_SIZE blockSize, CHUNK_FACTOR chunkFactor) :
        blockSize(blockSize),
        chunkFactor(chunkFactor)
    {}

    Knobs::Knobs() : 
        device(DEVICE::CPU),
        cpuKnobs() 
    {}

    Knobs::Knobs(CPUKnobs cpuKnobs) : 
        device(DEVICE::CPU),
        cpuKnobs(cpuKnobs)
    {}

    Knobs::Knobs(GPUKnobs gpuKnobs) : 
        device(DEVICE::GPU),
        gpuKnobs(gpuKnobs)
    {}
    
}
