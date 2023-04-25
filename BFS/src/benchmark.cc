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
#include "AppRegisterClient/Utils.h"

#include <margot/margot.hpp>

namespace po = boost::program_options;

po::options_description SetupOptions();
void SetupSignals();
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

    std::cout << "EVENT,TYPE,DEVICE,TIMESTAMP" << std::endl;
 

    //START: SETUP
    std::cout << "SETUP,START,CPU," << now() << std::endl;
    
    po::options_description desc(SetupOptions());
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);

    if (vm.count("help"))
    {
        std::cout << desc << "\n";
        return 0;
    }

    SetupSignals();

    long double targetThroughput = vm["target-throughput"].as<long double>();
    //Attach to controller
    struct app_data* data = registerAttach(
        vm["instance-name"].as<std::string>().c_str(),
        targetThroughput,
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

    std::cout << "SETUP,STOP,CPU," << now() << std::endl;
    //STOP: SETUP


    //Spinlock
    //START: WAIT REGISTRATION
    std::cout << "WAIT REGISTRATION,START,CPU," << now() << std::endl;
    while(true){
        if(isRegistered(data)){
            setTickStartTime(data);
            break;
        }
    }
    std::cout << "WAIT REGISTRATION,STOP,CPU," << now() << std::endl;
    //STOP: WAIT REGISTRATION


    int error = 0;
    while(!stop && !error)
    {
        //Read knobs
        //START: CONTROLLER PULL
        std::cout << "CONTROLLER PULL,START,CPU," << now() << std::endl;
        error = binarySemaphoreWait(dataSemId);
        cpuThreads = getNCpuCores(data);
        device = getUseGpu(data) ? BFS::Knobs::DEVICE::GPU : BFS::Knobs::DEVICE::CPU;
        error = binarySemaphorePost(dataSemId);
        deviceId = static_cast<unsigned int>(device);
        std::cout << "CONTROLLER PULL,STOP,CPU," << now() << std::endl;
        //STOP: CONTROLLER PULL

        //START: MARGOT PULL
        std::cout << "MARGOT PULL,START,CPU," << now() << std::endl;
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
        std::cout << "MARGOT PULL,STOP,CPU," << now() << std::endl;
        //STOP: MARGOT PULL
 
        //START: WIND UP
        std::cout << "WIND UP,START,CPU," << now() << std::endl;
        margot::bfs::start_monitors();
        Graph::Graph graph(GraphUtils::ReadGraphFile(vm["input-file"].as<std::string>()));
        std::unique_ptr<BFS::Knobs> knobs( 
            device == BFS::Knobs::DEVICE::GPU ?
            static_cast<BFS::Knobs*>(new BFS::GpuKnobs(gpuBlockSize, gpuChunkFactor, gpuOffsetsMem, gpuEdgesMem)) :
            static_cast<BFS::Knobs*>(new BFS::CpuKnobs(cpuThreads))
        );
        std::unique_ptr<BFS::BfsResult> bfs(knobs->buildBfs(graph, 0));
        std::cout << "WIND UP,STOP,CPU," << now() << std::endl;
        //STOP: WIND UP

        //START: KERNEL
        std::cout << "KERNEL,START," << BFS::Knobs::ToString(device) << "," << now() << std::endl;
        while(!bfs->kernel()){}
        std::cout << "KERNEL,STOP," << BFS::Knobs::ToString(device) << "," << now() << std::endl;
        //STOP: KERNEL
 
        //START: WIND DOWN
        std::cout << "WIND DOWN,START,CPU," << now() << std::endl;
        if(vm.count("output-file")){
            BFSUtils::WriteGraphResultFile(vm["output-file"].as<std::string>(), *bfs);
        }
        std::cout << "WIND DOWN,STOP,CPU," << now() << std::endl;
        //START: WIND DOWN

        //START: MARGOT PUSH
        std::cout << "MARGOT PUSH,START,CPU," << now() << std::endl;
        margot::bfs::stop_monitors();
        margot::bfs::push_custom_monitor_values();
        std::cout << "MARGOT PUSH,STOP,CPU," << now() << std::endl;
        //STOP: MARGOT PUSH


        //Add tick
        //START: CONTROLLER PUSH
        std::cout << "CONTROLLER PUSH,START,CPU," << now() << std::endl;
        autosleep(data, targetThroughput);
        error = binarySemaphoreWait(dataSemId);
        addTick(data, 1);
        error = binarySemaphorePost(dataSemId);
        std::cout << "CONTROLLER PUSH,STOP,CPU," << now() << std::endl;
        //STOP: CONTROLLER PUSH

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

void SetupSignals()
{
    auto stopBenchmark = [](int signal){
        std::cerr << std::endl;
        std::cerr << "Received signal: " << signal << std::endl;
        std::cerr << "Stopping benchmark" << std::endl;

        stop = true;
    };

    std::signal(SIGINT, stopBenchmark);
    std::signal(SIGTERM, stopBenchmark);

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
