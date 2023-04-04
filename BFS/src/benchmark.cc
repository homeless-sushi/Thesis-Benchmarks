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
void CastKnobs(
    unsigned int gpuBlockSizeExp,
    unsigned int gpuChunkFactorExp,
    unsigned int gpuOffsetsMemId,
    unsigned int gpuEdgesMemId,
    BFS::GpuKnobs::BLOCK_SIZE& gpuBlockSize, 
    BFS::GpuKnobs::CHUNK_FACTOR& gpuChunkFactor,
    BFS::GpuKnobs::MEMORY_TYPE& gpuOffsetsMem,
    BFS::GpuKnobs::MEMORY_TYPE& gpuEdgesMem
);

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
    struct app_data* data = registerAttach(
        vm["instance-name"].as<std::string>().c_str(),
        vm["target-throughput"].as<long double>(),
        1,
        false);
    int dataSemId = semget(getpid(), 1, 0);

    margot::init();

    unsigned int deviceId = 0;
    unsigned int gpuBlockSizeExp = 0;
    unsigned int gpuChunkFactorExp = 0;
    unsigned int gpuOffsetsMemId = 0;
    unsigned int gpuEdgesMemId = 0;

    BFS::Knobs::DEVICE device;
    unsigned int cpuThreads;
    BFS::GpuKnobs::BLOCK_SIZE gpuBlockSize; 
    BFS::GpuKnobs::CHUNK_FACTOR gpuChunkFactor;
    BFS::GpuKnobs::MEMORY_TYPE gpuOffsetsMem;
    BFS::GpuKnobs::MEMORY_TYPE gpuEdgesMem;

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
        device = getUseGpu(data) ? BFS::Knobs::DEVICE::GPU : BFS::Knobs::DEVICE::CPU;
        binarySemaphorePost(dataSemId);

        deviceId = static_cast<unsigned int>(device);
        
        if(margot::bfs::update(cpuThreads, deviceId, gpuBlockSizeExp, gpuChunkFactorExp, gpuEdgesMemId, gpuOffsetsMemId)){
            CastKnobs(
                gpuBlockSizeExp,
                gpuChunkFactorExp,
                gpuOffsetsMemId,
                gpuEdgesMemId,
                gpuBlockSize, 
                gpuChunkFactor,
                gpuOffsetsMem,
                gpuEdgesMem
            );
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
    ("instance-name", po::value<std::string>()->default_value("BFS"), "name of benchmark instance")
    ("target-throughput", po::value<long double>()->default_value(1.0), "target throughput for the kernel")
    ;

    return desc;
}

void CastKnobs(
    unsigned int gpuBlockSizeExp,
    unsigned int gpuChunkFactorExp,
    unsigned int gpuOffsetsMemId,
    unsigned int gpuEdgesMemId,
    BFS::GpuKnobs::BLOCK_SIZE& gpuBlockSize, 
    BFS::GpuKnobs::CHUNK_FACTOR& gpuChunkFactor,
    BFS::GpuKnobs::MEMORY_TYPE& gpuOffsetsMem,
    BFS::GpuKnobs::MEMORY_TYPE& gpuEdgesMem
)
{
    gpuBlockSize = static_cast<BFS::GpuKnobs::BLOCK_SIZE>(BFS::GpuKnobs::BLOCK_SIZE::BLOCK_32 << gpuBlockSizeExp);
    gpuChunkFactor = static_cast<BFS::GpuKnobs::CHUNK_FACTOR>(BFS::GpuKnobs::CHUNK_FACTOR::CHUNK_1 << gpuChunkFactorExp);
    gpuOffsetsMem = static_cast<BFS::GpuKnobs::MEMORY_TYPE>(gpuOffsetsMemId);
    gpuEdgesMem = static_cast<BFS::GpuKnobs::MEMORY_TYPE>(gpuEdgesMemId);
}
