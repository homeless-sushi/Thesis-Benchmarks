#ifndef HMLSS_KNOBS
#define HMLSS_KNOBS

namespace Knobs 
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

#endif //HMLSS_KNOBS
