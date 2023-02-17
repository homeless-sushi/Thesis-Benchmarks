#ifndef HMLSS_BFS_UTILS
#define HMLSS_BFS_UTILS

#include "HMLSS_Bfs/Bfs.h"

#include <string>

#include "HMLSS_Graph/Graph.h"

namespace BFSUtils
{
    int WriteGraphResultFile(std::string fileURL, BFS::BfsResult& bfs);
}

#endif //HMLSS_BFS_UTILS