#include <iostream>
#include <string>
#include <memory>

#include <boost/program_options.hpp>

#include "HMLSS_Graph/Graph.h"
#include "HMLSS_Graph/GraphUtils.h"
#include "HMLSS_Bfs/Bfs.h"
#include "HMLSS_Bfs/BfsKnobs.h"
#include "HMLSS_Bfs/BfsUtils.h"
#include "HMLSS_Bfs/BfsCuda.h"

namespace po = boost::program_options;

po::options_description SetupOptions();
BFS::Knobs* SetKnobs(
    BFS::Knobs::DEVICE device,
    unsigned int cpuNumThreads,
    BFS::GpuKnobs::BLOCK_SIZE gpuBlockSize,
    BFS::GpuKnobs::CHUNK_FACTOR gpuChunkFactor,
    BFS::GpuKnobs::MEMORY_TYPE gpuOffsetMem,
    BFS::GpuKnobs::MEMORY_TYPE gpuEdgeMem);

int main(int argc, char *argv[])
{
    po::options_description desc(SetupOptions());
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);


    std::string inputFileURL(vm["input-file"].as<std::string>());
    Graph::Graph graph(GraphUtils::ReadGraphFile(inputFileURL));

    BFS::Knobs* knobs = SetKnobs(
        BFS::Knobs::FromString(vm["device"].as<std::string>()),
        vm["cpu-num-threads"].as<unsigned int>(),
        static_cast<BFS::GpuKnobs::BLOCK_SIZE>(vm["gpu-block-dim"].as<unsigned int>()),
        static_cast<BFS::GpuKnobs::CHUNK_FACTOR>(vm["gpu-chunk-factor"].as<unsigned int>()),
        BFS::GpuKnobs::FromString(vm["gpu-offset-mem"].as<std::string>()),
        BFS::GpuKnobs::FromString(vm["gpu-edge-mem"].as<std::string>()));
    BFS::BfsResult* bfs = knobs->buildBfs(graph, 0);
    while(!bfs->kernel()){}
    if(vm.count("output-file")){
        BFSUtils::WriteGraphResultFile(vm["output-file"].as<std::string>(), *bfs);
    }
    
    delete knobs;
    delete bfs;
}

po::options_description SetupOptions()
{
    po::options_description desc("Allowed options");
    desc.add_options()
    ("help", "Display help message")
    ("input-file,I", po::value<std::string>(), "input file with graph description")
    ("output-file,O", po::value<std::string>(), "output file with bfs solution")
    ("device,D", po::value<std::string>()->default_value(std::string("CPU")), "CPU, GPU; default: CPU")
    ("cpu-num-threads", po::value<unsigned int>()->default_value(1), "number of CPU threads; default: 1")
    ("gpu-block-dim", po::value<unsigned int>()->default_value(32), "32,64,...,1024; default: 32")
    ("gpu-chunk-factor", po::value<unsigned int>()->default_value(1), "1,2,4,8; default: 1")
    ("gpu-offset-mem", po::value<std::string>()->default_value(std::string("DEVICE")), "DEVICE, TEXTURE; default: DEVICE")
    ("gpu-edge-mem", po::value<std::string>()->default_value(std::string("DEVICE")), "DEVICE, TEXTURE; default: DEVICE")
    ;

    return desc;
}

BFS::Knobs* SetKnobs(
    BFS::Knobs::DEVICE device,
    unsigned int cpuNumThreads,
    BFS::GpuKnobs::BLOCK_SIZE gpuBlockSize,
    BFS::GpuKnobs::CHUNK_FACTOR gpuChunkFactor,
    BFS::GpuKnobs::MEMORY_TYPE gpuOffsetMem,
    BFS::GpuKnobs::MEMORY_TYPE gpuEdgeMem)
{
    switch (device)
    {
        case BFS::Knobs::DEVICE::GPU:
            return new BFS::GpuKnobs(gpuBlockSize, gpuChunkFactor, gpuOffsetMem, gpuEdgeMem);
        
        default:
            return new BFS::CpuKnobs(cpuNumThreads);
    }
}