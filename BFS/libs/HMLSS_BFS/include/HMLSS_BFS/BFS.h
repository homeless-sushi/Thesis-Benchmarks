#ifndef HMLSS_BFS
#define HMLSS_BFS

#include "HMLSS_BFS/BFSKnobs.h"

#include <vector>

#include "HMLSS_Graph/Graph.h"

namespace BFS
{
    struct BFSResult
    {
            Graph::Graph& graph;
            unsigned int source;
            int currentCost;

            BFSResult(Graph::Graph& graph, unsigned int source);
            BFSResult(
                Graph::Graph& graph,
                unsigned int source,
                int currentCost,
                std::vector<int> costs);
            virtual ~BFSResult();
            
            virtual bool kernel(BFS::Knobs knobs);

            virtual const std::vector<int>& costs();

        private:
            std::vector<int> costs_;
    };
 }

#endif //HMLSS_BFS