#ifndef HMLSS_BFS_KNOBS
#define HMLSS_BFS_KNOBS

namespace BFS 
{
    struct CPUKnobs 
    {
        unsigned int nThreads;

        CPUKnobs();
        CPUKnobs(unsigned int nThreads);
    };

    struct GPUKnobs
    {
        enum BLOCK_SIZE
        {
            BLOCK_32 = 32,
            BLOCK_64 = 64,
            BLOCK_128 = 128,
            BLOCK_256 = 256,
            BLOCK_512 = 512,
            BLOCK_1024 = 1024
        };
        BLOCK_SIZE blockSize;

        enum CHUNK_FACTOR
        {
            CHUNK_1 = 1,
            CHUNK_2 = 2,
            CHUNK_4 = 4,
            CHUNK_8 = 8
        };
        CHUNK_FACTOR chunkFactor;

        enum MEMORY_TYPE
        {
            DEVICE_MEMORY = 0,
            TEXTURE_MEMORY = 1
        };
        MEMORY_TYPE edgeOffsets;
        MEMORY_TYPE edges;

        GPUKnobs(
            BLOCK_SIZE blockSize = BLOCK_32,
            CHUNK_FACTOR chunkFactor = CHUNK_1, 
            MEMORY_TYPE edgeOffsets = DEVICE_MEMORY,
            MEMORY_TYPE edges = DEVICE_MEMORY);
    };

    struct Knobs
    {
        enum DEVICE 
        {
            CPU, 
            GPU
        };
        DEVICE device;

        union 
        {
            CPUKnobs cpuKnobs;
            GPUKnobs gpuKnobs;
        };

        Knobs();
        Knobs(CPUKnobs knobs);
        Knobs(GPUKnobs knobs);
    };
}

#endif //HMLSS_BFS_KNOBS
