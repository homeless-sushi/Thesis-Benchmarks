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

int main(int argc, char *argv[])
{
    po::options_description desc(SetupOptions());
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);


    std::string inputFileURL(vm["input-file"].as<std::string>());
    Graph::Graph graph(GraphUtils::ReadGraphFile(inputFileURL));

    BFS::Knobs* knobs = new BFS::CpuKnobs();
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
    ;

    return desc;
}