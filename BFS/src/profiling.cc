#include <iostream>
#include <memory>
#include <string>

#include <cstdlib>

#include <boost/program_options.hpp>

#include "HMLSS_Graph/Graph.h"
#include "HMLSS_Graph/GraphUtils.h"
#include "HMLSS_Bfs/Bfs.h"
#include "HMLSS_Bfs/BfsKnobs.h"
#include "HMLSS_Bfs/BfsUtils.h"

#include <margot/margot.hpp>

namespace po = boost::program_options;

po::options_description SetupOptions();

int main(int argc, char *argv[])
{
    po::options_description desc(SetupOptions());
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);

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

    while(margot::bfs::context().manager.in_design_space_exploration()){
        
        if(margot::bfs::update(cpuThreadsExp, deviceId, gpuBlockSizeExp, gpuChunkFactorExp, gpuEdgesMemId, gpuOffsetsMemId)){
            device = static_cast<BFS::Knobs::DEVICE>(deviceId);
            cpuThreads = 1 << cpuThreadsExp;
            gpuBlockSize = static_cast<BFS::GpuKnobs::BLOCK_SIZE>(BFS::GpuKnobs::BLOCK_SIZE::BLOCK_32 << gpuBlockSizeExp);
            gpuChunkFactor = static_cast<BFS::GpuKnobs::CHUNK_FACTOR>(BFS::GpuKnobs::CHUNK_FACTOR::CHUNK_1 << gpuChunkFactorExp);
            gpuOffsetsMem = static_cast<BFS::GpuKnobs::MEMORY_TYPE>(gpuOffsetsMemId);
            gpuEdgesMem = static_cast<BFS::GpuKnobs::MEMORY_TYPE>(gpuEdgesMemId);
            margot::bfs::context().manager.configuration_applied();
        }

        margot::bfs::start_monitors();

        Graph::Graph graph(GraphUtils::ReadGraphFile(vm["input-file"].as<std::string>()));
        std::unique_ptr<BFS::Knobs> knobs( 
            device == BFS::Knobs::DEVICE::GPU ?
            static_cast<BFS::Knobs*>(new BFS::GpuKnobs(gpuBlockSize, gpuChunkFactor, gpuOffsetsMem, gpuEdgesMem)) :
            static_cast<BFS::Knobs*>(new BFS::CpuKnobs(cpuThreads))
        );
        std::unique_ptr<BFS::BfsResult> bfs(knobs->buildBfs(graph, 0));
        while(!bfs->kernel()){}
        if(vm.count("output-file")){
            BFSUtils::WriteGraphResultFile(vm["output-file"].as<std::string>(), *bfs);
        }
        
        margot::bfs::stop_monitors();
        margot::bfs::push_custom_monitor_values();
        margot::bfs::log();
    }
}

po::options_description SetupOptions()
{
    po::options_description desc("Allowed options");
    desc.add_options()
    ("help", "Display help message")
    ("input-file,I", po::value<std::string>(), "input file with graph description")
    ("output-file,O", po::value<std::string>(), "output file with bfs solution")
    ;

    return desc;
}