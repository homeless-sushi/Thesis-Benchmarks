#ifndef HMLSS_BFS_KNOBS
#define HMLSS_BFS_KNOBS

#include "HMLSS_Bfs/Bfs.h"

#include "utils/enum.h"

namespace BFS 
{
    struct Knobs
    {
        DEFINE_ENUM_WITH_STRING_CONVERSIONS(DEVICE, (CPU)(GPU))

        virtual BfsResult* buildBfs(Graph::Graph& graph, unsigned int source) = 0;
    };

    struct CpuKnobs : public Knobs
    {
        unsigned int nThreads;
        CpuKnobs(unsigned int nThreads = 1);

        BfsResult* buildBfs(Graph::Graph& graph, unsigned int source) override;
    };

    struct GpuKnobs : public Knobs
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

        DEFINE_ENUM_WITH_STRING_CONVERSIONS(MEMORY_TYPE, (DEVICE)(TEXTURE))
        MEMORY_TYPE edgeOffsets;
        MEMORY_TYPE edges;

        GpuKnobs(
            BLOCK_SIZE blockSize = BLOCK_32,
            CHUNK_FACTOR chunkFactor = CHUNK_1, 
            MEMORY_TYPE edgeOffsets = DEVICE,
            MEMORY_TYPE edges = DEVICE);

        BfsResult* buildBfs(Graph::Graph& graph, unsigned int source) override;
    };
}

#endif //HMLSS_BFS_KNOBS
