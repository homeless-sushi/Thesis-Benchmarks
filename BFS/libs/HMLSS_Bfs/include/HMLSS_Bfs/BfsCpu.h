#ifndef HMLSS_BFS_CPU
#define HMLSS_BFS_CPU

#include "HMLSS_Bfs/Bfs.h"
#include "HMLSS_Bfs/BfsKnobs.h"

#include <vector>

#include "cuda_runtime_api.h"

#include "HMLSS_Graph/Graph.h"

namespace BFS
{
    class BfsCpu : public BfsResult
    {
        public:
            BfsCpu(CpuKnobs knobs, Graph::Graph& graph, unsigned int source);
            virtual ~BfsCpu() override;

            virtual bool kernel() override;
            virtual const std::vector<int>& costs() override;

        private:
            CpuKnobs knobs_;
            
            std::vector<int> costs_;
    };
}

#endif //HMLSS_BFS_CPU