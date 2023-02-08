#include "HMLSS_Graph/Graph.h"
#include "HMLSS_Graph/GraphUtils.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

namespace GraphUtils 
{
    int ReadGraphFile(std::string fileURL, Graph::Graph& graph)
    {
	    std::ifstream graphFile(fileURL);
        if (!graphFile.is_open()){
            std::cout << "Error: Cannot open " << fileURL << std::endl;
            return -1;
        }
        
        std::string lineString;
        getline(graphFile, lineString);
        std::istringstream firstLineStream(lineString);
        unsigned int nVertices;
        firstLineStream >> nVertices;
        unsigned int nEdges;
        firstLineStream >> nEdges;
        graph = Graph::Graph(nVertices, nEdges);
        
        unsigned int vertexId = 0;
        unsigned int nEdgesRead = 0;
        graph.edgeOffsets[vertexId] = nEdgesRead;
        while(graphFile.good()){

            getline(graphFile, lineString);
            std::istringstream lineStream(lineString);
            while(lineStream.good()){
                lineStream >> graph.edges[nEdgesRead];
                ++nEdgesRead;
            }

            ++vertexId;
            graph.edgeOffsets[vertexId] = nEdgesRead;
        }
        graphFile.close();
        return 0;
    }

    int WriteGraphFile(std::string fileURL, Graph::Graph& graph)
    {
	    std::ofstream graphFile (fileURL);
        if (!graphFile.is_open()){
            std::cout << "Error: Cannot open " << fileURL << std::endl;
            return -1;
        }
        
        graphFile << graph.nVertices << " " << graph.nEdges << std::endl;
        
        for(unsigned int nodeId = 0; nodeId < graph.nVertices + 1; ++nodeId){
            const unsigned int startOffset = graph.edgeOffsets[nodeId];
            const unsigned int endOffset = graph.edgeOffsets[nodeId+1];
            for(unsigned int edgeId = startOffset; edgeId < endOffset; ++edgeId){
                graphFile << " " << graph.edges[edgeId];
            }
            graphFile << std::endl;
        }
        graphFile << std::endl;

        return 0;
    }
}