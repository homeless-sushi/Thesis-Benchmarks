#include "HMLSS_BFS/BFS.h"

#include <omp.h>

#include "HMLSS_Graph/Graph.h"

namespace BFS 
{
    BFSResult::BFSResult(Graph::Graph& graph, unsigned int source) :
        graph(graph),
        source{source},
        currentCost{0},
        costs_(graph.nVertices, -1)
    {
        costs_[source] = 0;
    }
    
    BFSResult::~BFSResult() = default;

    const std::vector<int>& BFSResult::costs() const { return costs_; }

    bool BFSResult::kernel(BFSKnobs::Knobs knobs) 
    {
        bool done = true;

        #pragma omp parallel for \
        num_threads(nThreads)
        for(int fromNode = 0; fromNode < graph.nVertices; fromNode++){
            if (costs_[fromNode] == currentCost){
                const int nodeEdgesStart = graph.edgeOffsets[fromNode];
                const int nodeEdgesStop = graph.edgeOffsets[fromNode+1];
                for(int edgeId = nodeEdgesStart; edgeId < nodeEdgesStop; edgeId++){
                    const int toNode = graph.edges[edgeId];
                    if(costs_[toNode] == -1){
                        costs_[toNode]=currentCost+1;
                        done=false;
                    }
                }
            }
        }

        currentCost++;
        return done;
    }
}
