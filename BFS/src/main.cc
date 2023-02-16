#include <iostream>
#include <string>
#include <memory>

#include "HMLSS_Graph/Graph.h"
#include "HMLSS_Graph/GraphUtils.h"
#include "HMLSS_BFS/BFS.h"
#include "HMLSS_BFS/BFSKnobs.h"
#include "HMLSS_BFS/BFSUtils.h"
#include "HMLSS_BFS/BFS_CUDA.h"
#include "HMLSS_BFS/BFSFactory.h"

BFS::BFSResult* migrate(BFS::BFSResult* bfs, BFS::Knobs prevKnobs, BFS::Knobs currKnobs);

BFS::Knobs getKnobs(BFS::Knobs prevKnobs);

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
    BFS::Knobs currKnobs = BFS::Knobs();
    BFS::BFSResult* bfs = BFS::buildBFS(currKnobs, graph, 0);
    while(true){
        BFS::Knobs prevKnobs = currKnobs;
        //currKnobs = getKnobs(prevKnobs);
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

inline BFS::BFSResult* migrate(BFS::BFSResult* bfs, BFS::Knobs prevKnobs, BFS::Knobs currKnobs)
{
    if(prevKnobs.device == currKnobs.device)
        return bfs;
    
    BFS::BFSResult* newBFS = BFS::buildBFS(currKnobs, bfs);
    delete bfs;
    return newBFS;
}

BFS::Knobs getKnobs(BFS::Knobs prevKnobs)
{
    switch (prevKnobs.device) 
    {
        case BFS::Knobs::DEVICE::GPU :
            return BFS::Knobs();

        default :
            return BFS::Knobs(BFS::GPUKnobs());
    }
}
