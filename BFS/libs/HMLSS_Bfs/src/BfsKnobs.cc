#include "HMLSS_Bfs/Bfs.h"
#include "HMLSS_Bfs/BfsKnobs.h"
#include "HMLSS_Bfs/BfsCpu.h"
#include "HMLSS_Bfs/BfsCuda.h"

namespace BFS 
{
    CpuKnobs::CpuKnobs(unsigned int nThreads) : nThreads(nThreads) {}
    BfsResult* CpuKnobs::buildBfs(Graph::Graph& graph, unsigned int source) {
        return new BfsCpu(*this, graph, source);
    }

    GpuKnobs::GpuKnobs(
        BLOCK_SIZE blockSize,
        CHUNK_FACTOR chunkFactor,
        MEMORY_TYPE edgeOffsets,
        MEMORY_TYPE edges) :
        blockSize(blockSize),
        chunkFactor(chunkFactor),
        edgeOffsets(edgeOffsets),
        edges(edges)
    {}   
    BfsResult* GpuKnobs::buildBfs(Graph::Graph& graph, unsigned int source) {
        return new BfsCuda(*this, graph, source);
    } 
}
