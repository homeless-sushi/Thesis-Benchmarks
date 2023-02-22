#ifndef HMLSS_GRAPH_UTILS
#define HMLSS_GRAPH_UTILS

#include "HMLSS_Graph/Graph.h"

#include <string>

namespace GraphUtils 
{
    Graph::Graph ReadGraphFile(std::string fileURL);
}

#endif //HMLSS_GRAPH_UTILS