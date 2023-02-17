#include "HMLSS_Bfs/Bfs.h"
#include "HMLSS_Bfs/BfsKnobs.h"
#include "HMLSS_Bfs/BfsCpu.h"

#include <iostream>

#include <omp.h>

#include "HMLSS_Graph/Graph.h"

namespace BFS 
{
    BfsCpu::BfsCpu(BFS::CpuKnobs knobs, Graph::Graph& graph, unsigned int source) :
        BfsResult(graph, source),
        knobs_(knobs),
        costs_(graph.nVertices, -1)
    {
        costs_[source] = 0;
    }
    BfsCpu::~BfsCpu() = default;

    const std::vector<int>& BfsCpu::costs() { return costs_; }

    bool BfsCpu::kernel() 
    {
        bool done = true;
        #pragma omp parallel for \
        num_threads(knobs_.nThreads)
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
