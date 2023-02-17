#include <iostream>
#include <string>
#include <memory>

#include "HMLSS_Graph/Graph.h"
#include "HMLSS_Graph/GraphUtils.h"
#include "HMLSS_Bfs/Bfs.h"
#include "HMLSS_Bfs/BfsKnobs.h"
#include "HMLSS_Bfs/BfsUtils.h"
#include "HMLSS_Bfs/BfsCuda.h"

int main(int argc, char *argv[])
{
    if(argc < 2){
        std::cout << "Error: Invalid arguments!" << std::endl;
        std::cout << "\tUsage: " << argv[0] << " graph.txt [result.txt]" << std::endl;
        return -1;
    }

    std::string inputFileURL(argv[1]);
    Graph::Graph graph;
    GraphUtils::ReadGraphFile(inputFileURL, graph);

    BFS::Knobs* knobs = new BFS::GpuKnobs();
    BFS::BfsResult* bfs = knobs->buildBfs(graph, 0);
    while(!bfs->kernel()){}
    if(argc == 3){
        BFSUtils::WriteGraphResultFile(argv[2], *bfs);
    }
    
    delete knobs;
    delete bfs;
}
