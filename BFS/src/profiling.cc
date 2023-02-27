#include <iostream>
#include <string>
#include <sstream>

#include <cstdlib>

#include "HMLSS_Bfs/BfsKnobs.h"

#include <margot/margot.hpp>

int main(int argc, char *argv[])
{
    if(argc < 4){
        std::cout << "Error: Invalid arguments!" << std::endl;
        std::cout << "\tUsage: " << argv[0] << "BfsProgram graph.txt res.txt" << std::endl;
        return -1;
    }
    std::string graph(argv[2]);

    margot::init();
    margot::bfs::context().manager.wait_for_knowledge(10);

    unsigned int deviceId = 0;
    BFS::Knobs::DEVICE device = static_cast<BFS::Knobs::DEVICE>(deviceId);
    unsigned int cpuThreads = 1;
    unsigned int cpuThreadsExp = 0;
    BFS::GpuKnobs::BLOCK_SIZE gpuBlockSize = BFS::GpuKnobs::BLOCK_SIZE::BLOCK_32;
    unsigned int gpuBlockSizeExp = 0;
    BFS::GpuKnobs::CHUNK_FACTOR gpuChunkFactor = BFS::GpuKnobs::CHUNK_FACTOR::CHUNK_1;
    unsigned int gpuChunkFactorExp = 0;
    unsigned int gpuOffsetsMemId = 0;
    BFS::GpuKnobs::MEMORY_TYPE gpuOffsetsMem = static_cast<BFS::GpuKnobs::MEMORY_TYPE>(gpuOffsetsMemId);
    unsigned int gpuEdgesMemId = 0;
    BFS::GpuKnobs::MEMORY_TYPE gpuEdgesMem = static_cast<BFS::GpuKnobs::MEMORY_TYPE>(gpuEdgesMemId);

    while(margot::bfs::context().manager.in_design_space_exploration())
    {
        if(margot::bfs::update(cpuThreadsExp, deviceId, gpuBlockSizeExp, gpuChunkFactorExp, gpuEdgesMemId, gpuOffsetsMemId)){
            device = static_cast<BFS::Knobs::DEVICE>(deviceId);
            cpuThreads = 1 << cpuThreadsExp;
            gpuBlockSize = static_cast<BFS::GpuKnobs::BLOCK_SIZE>(BFS::GpuKnobs::BLOCK_SIZE::BLOCK_32 << gpuBlockSizeExp);
            gpuChunkFactor = static_cast<BFS::GpuKnobs::CHUNK_FACTOR>(BFS::GpuKnobs::CHUNK_FACTOR::CHUNK_1 << gpuChunkFactorExp);
            gpuOffsetsMem = static_cast<BFS::GpuKnobs::MEMORY_TYPE>(gpuOffsetsMemId);
            gpuEdgesMem = static_cast<BFS::GpuKnobs::MEMORY_TYPE>(gpuEdgesMemId);
            margot::bfs::context().manager.configuration_applied();
        }

        std::stringstream command;
        command << "./" << argv[1];
        command << " -I " << argv[2];
        command << " -O " << argv[3];
        command << " -D " << BFS::Knobs::ToString(device);
        command << " --cpu-num-threads " << cpuThreads;
        command << " --gpu-block-dim " << gpuBlockSize;
        command << " --gpu-chunk-factor " << gpuChunkFactor;
        command << " --gpu-offset-mem " << BFS::GpuKnobs::ToString(gpuOffsetsMem);
        command << " --gpu-edge-mem " << BFS::GpuKnobs::ToString(gpuEdgesMem);

        margot::bfs::start_monitors();

        std::system(command.str().c_str());
        
        margot::bfs::stop_monitors();
        margot::bfs::push_custom_monitor_values();
        margot::bfs::log();
    }
}
