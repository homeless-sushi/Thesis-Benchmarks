#include "HMLSS_BFS/BFS.h"

#include <vector>

namespace BFS
{
    BFSResult::BFSResult(unsigned int nVertices, unsigned int source) :
        source{source}, 
        costs(nVertices, -1),
        currentCost{0}
    {
        costs[source] = 0;
    }

    //void BFSMigrateCPUtoGPU(Graph::Graph& graph, BFS::BFSResult& bfs);
    //void BFSMigrateGPUtoCPU(Graph::Graph& graph, BFS::BFSResult& bfs);
}
