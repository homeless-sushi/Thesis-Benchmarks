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

#include <margot/margot.hpp>

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

    margot::init();
    margot::bfs::context().manager.wait_for_knowledge(10);
    
    unsigned int deviceNumber = 0;
    BFS::Knobs::DEVICE device = BFS::Knobs::DEVICE::CPU;
    unsigned int cpuThreadsExp = 0;
    unsigned int cpuThreads = 1;
    unsigned int gpuBlockSizeExp = 0;
    BFS::GPUKnobs::BLOCK_SIZE gpuBlockSize = BFS::GPUKnobs::BLOCK_SIZE::BLOCK_32;
    unsigned int gpuChunkFactorExp = 0;
    BFS::GPUKnobs::CHUNK_FACTOR gpuChunkFactor = BFS::GPUKnobs::CHUNK_FACTOR::CHUNK_1;
    unsigned int gpuEdgeOffsetsMemoryType = 0;
    BFS::GPUKnobs::MEMORY_TYPE gpuEdgeOffsetsMemory = BFS::GPUKnobs::MEMORY_TYPE::DEVICE_MEMORY;
    unsigned int gpuEdgesMemoryType = 0;
    BFS::GPUKnobs::MEMORY_TYPE gpuEdgesMemory = BFS::GPUKnobs::MEMORY_TYPE::DEVICE_MEMORY;

    while(margot::bfs::context().manager.in_design_space_exploration())
    {
        if(margot::bfs::update(deviceNumber, cpuThreadsExp, gpuBlockSizeExp, gpuChunkFactorExp, gpuEdgeOffsetsMemoryType, gpuEdgesMemoryType)){
            device = static_cast<BFS::Knobs::DEVICE>(deviceNumber);
            cpuThreads = 1 << cpuThreadsExp;
            gpuBlockSize = static_cast<BFS::GPUKnobs::BLOCK_SIZE>(BFS::GPUKnobs::BLOCK_SIZE::BLOCK_32 << gpuBlockSizeExp);
            gpuChunkFactor = static_cast<BFS::GPUKnobs::CHUNK_FACTOR>(BFS::GPUKnobs::CHUNK_FACTOR::CHUNK_1 << gpuChunkFactorExp);
            gpuEdgeOffsetsMemoryType = static_cast<BFS::GPUKnobs::MEMORY_TYPE>(gpuEdgeOffsetsMemoryType);
            gpuEdgesMemory = static_cast<BFS::GPUKnobs::MEMORY_TYPE>(gpuEdgesMemoryType);
            margot::bfs::context().manager.configuration_applied();
        }

        margot::bfs::start_monitors();
        BFS::Knobs currKnobs;
        switch (device)
        {
            case BFS::Knobs::DEVICE::GPU :
                currKnobs = BFS::CPUKnobs(cpuThreads);
                break;

            default :
                currKnobs = BFS::GPUKnobs(gpuBlockSize, gpuChunkFactor, gpuEdgeOffsetsMemory, gpuEdgesMemory);
        }
        BFS::BFSResult* bfs = BFS::buildBFS(currKnobs, graph, 0);
        BFS::Knobs prevKnobs = currKnobs;
        bfs = migrate(bfs, prevKnobs, currKnobs);
        margot::bfs::start_monitors();
        while(true){
            if(bfs->kernel(currKnobs))
                break;
        }
        margot::bfs::stop_monitors();
        margot::bfs::push_custom_monitor_values();
        margot::bfs::log();
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        delete bfs;
    }
}

inline BFS::BFSResult* migrate(BFS::BFSResult* bfs, BFS::Knobs prevKnobs, BFS::Knobs currKnobs)
{
    if(prevKnobs.device == currKnobs.device)
        return bfs;
    
    BFS::BFSResult* newBFS = BFS::buildBFS(currKnobs, bfs);
    delete bfs;
    return newBFS;
}
