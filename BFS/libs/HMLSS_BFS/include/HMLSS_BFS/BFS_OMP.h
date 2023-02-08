#ifndef HMLSS_BFS_OMP
#define HMLSS_BFS_OMP

#include "HMLSS_BFS/BFS.h"

#include <vector>

#include "HMLSS_Graph/Graph.h"

namespace BFS
{
    bool BFSOMPKernel(const Graph::Graph& graph, BFS::BFSResult& bfs, unsigned int nThreads = 1) ;
}

#endif //HMLSS_BFS_OMP