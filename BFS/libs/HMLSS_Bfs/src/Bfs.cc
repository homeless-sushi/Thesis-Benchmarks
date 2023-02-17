#include "HMLSS_Bfs/Bfs.h"

#include "HMLSS_Graph/Graph.h"

namespace BFS 
{
    BfsResult::BfsResult(Graph::Graph& graph, unsigned int source) :
        graph(graph),
        source{source},
        currentCost{0}
    {}
    BfsResult::~BfsResult() = default;
}
