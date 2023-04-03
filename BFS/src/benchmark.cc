#include <iostream>
#include <memory>
#include <string>
#include <sstream>

#include <cmath>
#include <csignal>
#include <cstdlib>

#include <unistd.h>

#include <boost/program_options.hpp>

#include "HMLSS_Graph/Graph.h"
#include "HMLSS_Graph/GraphUtils.h"
#include "HMLSS_Bfs/Bfs.h"
#include "HMLSS_Bfs/BfsKnobs.h"
#include "HMLSS_Bfs/BfsUtils.h"

#include "AppRegisterCommon/AppRegister.h"
#include "AppRegisterCommon/Semaphore.h"

#include "AppRegisterClient/AppRegister.h"

#include <margot/margot.hpp>

namespace po = boost::program_options;

po::options_description SetupOptions();

bool stop = false;

int main(int argc, char *argv[])
{
    po::options_description desc(SetupOptions());
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);

    //Stop when Ctrl+C is called
    std::signal(SIGINT, [](int signal){
        if (signal == SIGINT) {
            std::cout << std::endl << "Client stop" << std::endl;
            stop = true;
        }
    });

    //Attach to controller
    struct app_data* data = registerAttach("BFS", 1, 1, false);

    margot::init();

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

    int dataSemId = semget(getpid(), 1, 0);

    //Spinlock
    while(true){
        if(isRegistered(data)){
            setTickStartTime(data);
            break;
        }
    }

    while(!stop)
    {
        //Read knobs
        binarySemaphoreWait(dataSemId);
        cpuThreads = getNCpuCores(data);
        cpuThreadsExp = log2(cpuThreads);
        device = getUseGpu(data) ? BFS::Knobs::DEVICE::GPU : BFS::Knobs::DEVICE::CPU; 
        deviceId = static_cast<unsigned int>(device);
        binarySemaphorePost(dataSemId);
        
        if(margot::bfs::update(cpuThreadsExp, deviceId, gpuBlockSizeExp, gpuChunkFactorExp, gpuEdgesMemId, gpuOffsetsMemId)){
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

        //Add tick
        binarySemaphoreWait(dataSemId);
        addTick(data, 1);
        binarySemaphorePost(dataSemId);
    }

    //Detach from controller
    registerDetach(data);
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
