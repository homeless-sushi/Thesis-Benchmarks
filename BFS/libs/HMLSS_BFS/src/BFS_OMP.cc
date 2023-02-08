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
        for(int vertexId = 0; vertexId < graph.nVertices; vertexId++){
            if (bfs.frontier[vertexId]){
                const int currCost = bfs.costs[vertexId];
                const int nodeEdgesStart = graph.edgeOffsets[vertexId];
                const int nodeEdgesStop = graph.edgeOffsets[vertexId+1];
                for(int edgeId = nodeEdgesStart; edgeId < nodeEdgesStop; edgeId++){
                    const int destinationvertexId = graph.edges[edgeId];
                    if(!bfs.visited[destinationvertexId]){
                        bfs.visited[destinationvertexId]=true;
                        bfs.costs[destinationvertexId]=currCost+1;
                        bfs.nextFrontier[destinationvertexId]=true;
                        done=false;
                    }
                }
            }
        }

        bfs.frontier.swap(bfs.nextFrontier);
        std::fill(bfs.nextFrontier.begin(), bfs.nextFrontier.end(), false);

        return done;
    }
}

