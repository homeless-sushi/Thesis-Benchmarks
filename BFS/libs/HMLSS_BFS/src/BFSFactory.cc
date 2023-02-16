#include "HMLSS_BFS/BFSKnobs.h"
#include "HMLSS_BFS/BFS.h"
#include "HMLSS_BFS/BFS_CUDA.h"
#include "HMLSS_BFS/BFSFactory.h"

#include <vector>

#include "HMLSS_Graph/Graph.h"

namespace BFS
{
    BFS::BFSResult* buildBFS(
        BFS::Knobs knobs,
        Graph::Graph& graph,
        unsigned int source)
    {
        switch (knobs.device)
        {
            case BFS::Knobs::DEVICE::GPU :
                return new BFS::BFSCUDA(graph, source);
            
            default:
                return new BFS::BFSResult(graph, source);
        }
    };

    BFS::BFSResult* buildBFS(
        BFS::Knobs knobs,
        Graph::Graph& graph,
        unsigned int source,
        int currentCost,
        std::vector<int> costs)
    {
        switch (knobs.device)
        {
            case BFS::Knobs::DEVICE::GPU :
                return new BFS::BFSCUDA(graph, source, currentCost, costs);
            
            default:
                return new BFS::BFSResult(graph, source, currentCost, costs);
        }
    };

    BFS::BFSResult* buildBFS(
        BFS::Knobs knobs,
        BFS::BFSResult* bfs)
    {
        return buildBFS(
            knobs,
            bfs->graph,
            bfs->source,
            bfs->currentCost,
            bfs->costs());
    };
}