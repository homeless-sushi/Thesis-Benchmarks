#include "HMLSS_Graph/Graph.h"

namespace Graph 
{
    Graph::Graph()=default;
    Graph::Graph(unsigned int nVertices, unsigned int nEdges) :
        nEdges(nEdges),
        nVertices(nVertices) 
        {
            edgeOffsets.reserve(nVertices+1);
            edges.reserve(nEdges);
        }
}