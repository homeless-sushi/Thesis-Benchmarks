#ifndef HMLSS_BFS_UTILS
#define HMLSS_BFS_UTILS

#include "HMLSS_Bfs/Bfs.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "HMLSS_Graph/Graph.h"

namespace BFSUtils
{
    int WriteGraphResultFile(std::string fileURL, BFS::BfsResult& bfs)
    {
	    std::ofstream resultFile (fileURL);
        if (!resultFile.is_open()){
            std::cerr << "Error: Cannot open " << fileURL << std::endl;
            return -1;
        }

        for (int cost : bfs.costs()){
            resultFile << cost << std::endl;
        }

        return 0;
    }
}

#endif //HMLSS_BFS_UTILS