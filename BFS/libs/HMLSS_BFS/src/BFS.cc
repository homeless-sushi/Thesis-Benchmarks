#include "HMLSS_BFS/BFS.h"

#include <vector>

namespace BFS
{
    BFSResult::BFSResult(unsigned int nVertices, unsigned int source) :
        source{source}, 
        visited(nVertices, false),
        costs(nVertices, -1),
        frontier(nVertices, false),
        nextFrontier(nVertices, false) 
    {
        visited[source] = true;
        costs[source] = 0;
        frontier[source] = true;
    }

    //void BFSMigrateCPUtoGPU(Graph::Graph& graph, BFS::BFSResult& bfs);
    //void BFSMigrateGPUtoCPU(Graph::Graph& graph, BFS::BFSResult& bfs);
}
