#include "HMLSS_BFS/BFSKnobs.h"

namespace BFS 
{
    CPUKnobs::CPUKnobs() : nThreads(1) {}
    CPUKnobs::CPUKnobs(unsigned int nThreads) : nThreads(nThreads) {}

    GPUKnobs::GPUKnobs(
        BLOCK_SIZE blockSize,
        CHUNK_FACTOR chunkFactor,
        MEMORY_TYPE edgeOffsets,
        MEMORY_TYPE edges) :
        blockSize(blockSize),
        chunkFactor(chunkFactor),
        edgeOffsets(edgeOffsets),
        edges(edges)
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
