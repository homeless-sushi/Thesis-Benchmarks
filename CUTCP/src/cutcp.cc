#include <iostream>
#include <string>

#include <Atom/Atom.h>
#include <Atom/Lattice.h>
#include <Atom/Utils.h>
#include <Cutcp/Cutcp.h>
#include <Cutcp/Utils.h>
#include <Debug/Debug.h>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

po::options_description SetupOptions();

int main(int argc, char *argv[])
{
    po::options_description desc(SetupOptions());
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);

    float spacing = 0.5;
    float padding = 0.5;
    float cutoff = 12.;		
    float exclusionCutoff = 1.;

    std::string inputFileURL(vm["input-file"].as<std::string>());
    std::vector<Atom::Atom> atoms = Atom::ReadAtomFile(inputFileURL);
    Debug::WriteAtoms(vm["output-file"].as<std::string>(), atoms);

    Atom::Vec3 minCoords;
    Atom::Vec3 maxCoords;
    Atom::GetAtomBounds(atoms, minCoords, maxCoords);
    std::cout << "extent of domain is:" << std::endl;
    std::cout << "\t" << "minimum: " << minCoords.x << " " << minCoords.y << " " << minCoords.z << std::endl;
    std::cout << "\t" << "maximum: " << maxCoords.x << " " << maxCoords.y << " " << maxCoords.z << std::endl;

    Atom::Vec3 paddingVec(padding);
    minCoords = minCoords - paddingVec;
    maxCoords = maxCoords + paddingVec;
    std::cout << "padding domain by " << padding << " Angstroms:" << std::endl;
    std::cout << "domain lenghts are " << maxCoords.x-minCoords.x << " by " << maxCoords.y-minCoords.y << " by " << maxCoords.z-minCoords.z << std::endl;

    Lattice::LatticeDim latticeDim(minCoords, maxCoords, spacing);
    Lattice::Lattice lattice(latticeDim);
    
    Cutcp::CutoffPotential(lattice, cutoff, atoms);
    Debug::WriteLattice(vm["output-file"].as<std::string>(), lattice);

    Cutcp::RemoveExclusions(lattice, exclusionCutoff, atoms);
    Debug::WriteLattice(vm["output-file"].as<std::string>(), lattice);
    return 0;
    //if(vm.count("output-file")){
    //    Cutcp::WriteLatticeFile(vm["output-file"].as<std::string>(), lattice);
    //}
}

po::options_description SetupOptions()
{
    po::options_description desc("Allowed options");
    desc.add_options()
    ("help", "Display help message")
    ("input-file,I", po::value<std::string>(), "input file with graph description")
    ("output-file,O", po::value<std::string>(), "output file with bfs solution")
    //("device,D", po::value<std::string>()->default_value(std::string("CPU")), "CPU, GPU; default: CPU")
    //("cpu-num-threads", po::value<unsigned int>()->default_value(1), "number of CPU threads; default: 1")
    ;

    return desc;
}