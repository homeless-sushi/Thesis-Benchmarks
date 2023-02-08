#ifndef HMLSS_GRAPH_UTILS
#define HMLSS_GRAPH_UTILS

#include "HMLSS_Graph/Graph.h"

#include <string>

namespace GraphUtils 
{
    int ReadGraphFile(std::string fileURL, Graph::Graph& graph);
}

#endif //HMLSS_GRAPH_UTILS