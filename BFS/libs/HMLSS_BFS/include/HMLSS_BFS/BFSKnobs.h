#ifndef HMLSS_BFS_KNOBS
#define HMLSS_BFS_KNOBS

namespace BFSKnobs 
{
    enum DEVICE 
    {
        CPU, 
        GPU
    };

    struct CPUKnobs 
    {
        unsigned int nThreads;

        CPUKnobs();
        CPUKnobs(unsigned int nThreads);
    };

    struct GPUKnobs
    {
        unsigned int blockSize;
        bool textureMem;
    };

    struct Knobs
    {
        DEVICE device;
        union 
        {
            CPUKnobs cpuKnobs;
            GPUKnobs gpuKnobs;
        };

        Knobs();
    };
}

#endif //HMLSS_BFS_KNOBS
