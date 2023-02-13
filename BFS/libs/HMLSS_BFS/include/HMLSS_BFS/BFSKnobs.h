#ifndef HMLSS_BFS_KNOBS
#define HMLSS_BFS_KNOBS

namespace BFSKnobs 
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

        GPUKnobs(BLOCK_SIZE blockSize = BLOCK_32, CHUNK_FACTOR chunkFactor = CHUNK_1);
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
