#ifndef HMLSS_BFS
#define HMLSS_BFS

#include <vector>

#include "HMLSS_Graph/Graph.h"

namespace BFS
{
    struct BFSResult
    {
            unsigned int source;
            int currentCost;
            
            std::vector<int> costs;

            BFSResult(unsigned int nVertices, unsigned int source);
    };

    //void BFSMigrateCPUtoGPU(Graph::Graph& graph, BFS::BFSResult& bfs);
    //void BFSMigrateGPUtoCPU(Graph::Graph& graph, BFS::BFSResult& bfs);
}

#endif //HMLSS_BFS