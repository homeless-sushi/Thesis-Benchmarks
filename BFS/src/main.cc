#include <iostream>
#include <string>
#include <memory>

#include "HMLSS_Graph/Graph.h"
#include "HMLSS_Graph/GraphUtils.h"
#include "HMLSS_BFS/BFS.h"
#include "HMLSS_BFS/BFSKnobs.h"
#include "HMLSS_BFS/BFSUtils.h"
#include "HMLSS_BFS/BFS_CUDA.h"

BFS::BFSResult* migrate(BFS::BFSResult* bfs, BFSKnobs::Knobs prevKnobs, BFSKnobs::Knobs currKnobs);

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
    BFSKnobs::Knobs currKnobs = BFSKnobs::Knobs();
    BFS::BFSResult* bfs = new BFS::BFSResult(graph, 0);
    while(true){
        BFSKnobs::Knobs prevKnobs = currKnobs;
        currKnobs = BFSKnobs::Knobs(BFSKnobs::GPUKnobs());
        //get knobs from monitor
        //get knobs from margot
        bfs = migrate(bfs, prevKnobs, currKnobs);
        if(bfs->kernel(currKnobs))
            break;
    }

    if(argc == 3){
        BFSUtils::WriteGraphResultFile(argv[2], *bfs);
    }

    free(bfs);
}

BFS::BFSResult* migrate(BFS::BFSResult* bfs, BFSKnobs::Knobs prevKnobs, BFSKnobs::Knobs currKnobs)
{
    if(prevKnobs.device == currKnobs.device)
        return bfs;
    
    switch (currKnobs.device) 
    {
        case BFSKnobs::Knobs::DEVICE::GPU :
            return new BFS::BFSCUDA(bfs->graph, bfs->source);

        default :
            return new BFS::BFSResult(bfs->graph, bfs->source);
    }
}
