#ifndef HMLSS_BFS_UTILS
#define HMLSS_BFS_UTILS

#include "HMLSS_BFS/BFS.h"

#include <string>

#include "HMLSS_Graph/Graph.h"

namespace BFSUtils
{
    int WriteGraphResultFile(std::string fileURL, const BFS::BFSResult& bfs);
}

#endif //HMLSS_BFS_UTILS