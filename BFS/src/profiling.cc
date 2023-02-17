#include <iostream>
#include <string>
#include <memory>

#include "HMLSS_Graph/Graph.h"
#include "HMLSS_Graph/GraphUtils.h"
#include "HMLSS_Bfs/Bfs.h"
#include "HMLSS_Bfs/BfsKnobs.h"
#include "HMLSS_Bfs/BfsUtils.h"
#include "HMLSS_Bfs/BfsCuda.h"

#include <margot/margot.hpp>

int main(int argc, char *argv[])
{
    if(argc < 2){
        std::cout << "Error: Invalid arguments!" << std::endl;
        std::cout << "\tUsage: " << argv[0] << " graph.txt [result.txt]" << std::endl;
        return -1;
    }

    std::string inputFileURL(argv[1]);
    Graph::Graph graph(GraphUtils::ReadGraphFile(inputFileURL));

    margot::init();
    margot::bfs::context().manager.wait_for_knowledge(10);

    unsigned int deviceNumber = 0;
    BFS::Knobs::DEVICE device = BFS::Knobs::DEVICE::CPU;
    unsigned int cpuThreadsExp = 0;
    unsigned int cpuThreads = 1;
    unsigned int gpuBlockSizeExp = 0;
    BFS::GpuKnobs::BLOCK_SIZE gpuBlockSize = BFS::GpuKnobs::BLOCK_SIZE::BLOCK_32;
    unsigned int gpuChunkFactorExp = 0;
    BFS::GpuKnobs::CHUNK_FACTOR gpuChunkFactor = BFS::GpuKnobs::CHUNK_FACTOR::CHUNK_1;
    unsigned int gpuEdgeOffsetsMemoryType = 0;
    BFS::GpuKnobs::MEMORY_TYPE gpuEdgeOffsetsMemory = BFS::GpuKnobs::MEMORY_TYPE::DEVICE_MEMORY;
    unsigned int gpuEdgesMemoryType = 0;
    BFS::GpuKnobs::MEMORY_TYPE gpuEdgesMemory = BFS::GpuKnobs::MEMORY_TYPE::DEVICE_MEMORY;

    while(margot::bfs::context().manager.in_design_space_exploration())
    {
        if(margot::bfs::update(cpuThreadsExp, deviceNumber, gpuBlockSizeExp, gpuChunkFactorExp, gpuEdgesMemoryType, gpuEdgeOffsetsMemoryType)){
            device = static_cast<BFS::Knobs::DEVICE>(deviceNumber);
            cpuThreads = 1 << cpuThreadsExp;
            gpuBlockSize = static_cast<BFS::GpuKnobs::BLOCK_SIZE>(BFS::GpuKnobs::BLOCK_SIZE::BLOCK_32 << gpuBlockSizeExp);
            gpuChunkFactor = static_cast<BFS::GpuKnobs::CHUNK_FACTOR>(BFS::GpuKnobs::CHUNK_FACTOR::CHUNK_1 << gpuChunkFactorExp);
            gpuEdgeOffsetsMemoryType = static_cast<BFS::GpuKnobs::MEMORY_TYPE>(gpuEdgeOffsetsMemoryType);
            gpuEdgesMemory = static_cast<BFS::GpuKnobs::MEMORY_TYPE>(gpuEdgesMemoryType);
            margot::bfs::context().manager.configuration_applied();
        }

        BFS::Knobs* knobs;
        switch (device)
        {
            case BFS::Knobs::DEVICE::GPU :
                knobs = new BFS::CpuKnobs(cpuThreads);
                break;

            default :
                knobs = new BFS::GpuKnobs(gpuBlockSize, gpuChunkFactor, gpuEdgeOffsetsMemory, gpuEdgesMemory);
        }

        margot::bfs::start_monitors();

        BFS::BfsResult* bfs = knobs->buildBfs(graph, 0);
        while(!bfs->kernel()){}
        if(argc == 3){
            BFSUtils::WriteGraphResultFile(argv[2], *bfs);
        }
        delete knobs;
        delete bfs;

        margot::bfs::stop_monitors();
        margot::bfs::push_custom_monitor_values();
        margot::bfs::log();
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
}
