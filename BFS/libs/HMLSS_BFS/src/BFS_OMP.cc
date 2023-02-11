#include "HMLSS_BFS/BFS.h"
#include "HMLSS_BFS/BFS_OMP.h"

#include <omp.h>

#include "HMLSS_Graph/Graph.h"

namespace BFS 
{
    bool BFSOMPKernel(const Graph::Graph& graph, BFS::BFSResult& bfs, unsigned int nThreads) 
    {
        bool done = true;

        #pragma omp parallel for \
        num_threads(nThreads)
        for(int fromNode = 0; fromNode < graph.nVertices; fromNode++){
            if (bfs.costs[fromNode] == bfs.currentCost){
                const int nodeEdgesStart = graph.edgeOffsets[fromNode];
                const int nodeEdgesStop = graph.edgeOffsets[fromNode+1];
                for(int edgeId = nodeEdgesStart; edgeId < nodeEdgesStop; edgeId++){
                    const int toNode = graph.edges[edgeId];
                    if(bfs.costs[toNode] == -1){
                        bfs.costs[toNode]=bfs.currentCost+1;
                        done=false;
                    }
                }
            }
        }

        bfs.currentCost++;
        return done;
    }
}

