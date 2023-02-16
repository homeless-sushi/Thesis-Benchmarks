#ifndef HMLSS_BFS_FACTORY
#define HMLSS_BFS_FACTORY

#include "HMLSS_BFS/BFSKnobs.h"
#include "HMLSS_BFS/BFS.h"

#include <vector>

#include "HMLSS_Graph/Graph.h"

namespace BFS
{
    BFS::BFSResult* buildBFS(
        BFS::Knobs knobs,
        Graph::Graph& graph,
        unsigned int source);

    BFS::BFSResult* buildBFS(
        BFS::Knobs knobs,
        Graph::Graph& graph,
        unsigned int source,
        int currentCost,
        std::vector<int>& costs);

    BFS::BFSResult* buildBFS(
        BFS::Knobs knobs,
        BFS::BFSResult* bfs);
}

#endif //HMLSS_BFS_FACTORY