#ifndef HMLSS_BFS
#define HMLSS_BFS

#include <vector>

#include "HMLSS_Graph/Graph.h"

namespace BFS
{
    class BfsResult
    {
        public:
            Graph::Graph& graph;
            unsigned int source;
            int currentCost;           

            BfsResult(Graph::Graph& graph, unsigned int source);
            virtual ~BfsResult();
            virtual bool kernel() = 0;
            virtual const std::vector<int>& costs() = 0;
    };
 }

#endif //HMLSS_BFS