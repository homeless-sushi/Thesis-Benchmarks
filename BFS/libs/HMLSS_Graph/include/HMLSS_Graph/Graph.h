#ifndef HMLSS_GRAPH
#define HMLSS_GRAPH

#include <vector>

namespace Graph 
{

    class Graph
    {
        public:
            unsigned int nVertices; // number of vertices
            unsigned int nEdges; // number of edges
            std::vector<unsigned int> edgeOffsets; //there are n_vertex+1 element, so that (edge_offset[i+1]-edge_offset[i]) is the degree of i
            std::vector<unsigned int> edges; //the vertexes adjacent to vertex i begin at edge_list[edge_offset[i]]

            Graph();
            Graph(unsigned int nVertices, unsigned int nEdges);
    };
}

#endif //HMLSS_GRAPH