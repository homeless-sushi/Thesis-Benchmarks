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
            virtual ~BFSResult();

            virtual bool kernel(BFSKnobs::Knobs knobs);

            virtual const std::vector<int>& costs() const;

        private:
            std::vector<int> costs_;
    };
 }

#endif //HMLSS_BFS