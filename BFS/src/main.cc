#include <string>

#include "knobs.h"

#include <iostream>

#include "HMLSS_Graph/Graph.h"
#include "HMLSS_Graph/GraphUtils.h"
#include "HMLSS_BFS/BFS.h"
#include "HMLSS_BFS/BFS_OMP.h"
#include "HMLSS_BFS/BFSUtils.h"

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
    BFS::BFSResult bfs(graph.nVertices, 0);
    //connect_to_monitor
        //get initial resources
    Knobs::Knobs currKnobsSetting = Knobs::Knobs();
    bool done = false;
    while(!done){
        done = true;
        Knobs::Knobs prevKnobsSetting = currKnobsSetting;
        //get knobs from monitor
        //get knobs from margot
        if(currKnobsSetting.device != prevKnobsSetting.device){
            if(currKnobsSetting.device == Knobs::DEVICE::CPU){
                //TODO
            }
            if(currKnobsSetting.device == Knobs::DEVICE::GPU){
                //TODO
            }
        }

        if(currKnobsSetting.device == Knobs::DEVICE::CPU){
            done = BFS::BFSOMPKernel(graph, bfs, currKnobsSetting.cpuKnobs.nThreads);
        }
        //if(currKnobsSetting.device == Knobs::DEVICE::GPU){
        //
        //}
    }

    if(argc == 3){
        BFSUtils::WriteGraphResultFile(argv[2], bfs);
    }
}
