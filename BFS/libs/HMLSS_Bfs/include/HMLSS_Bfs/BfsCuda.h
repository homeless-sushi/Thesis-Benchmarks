#ifndef HMLSS_BFS_CUDA
#define HMLSS_BFS_CUDA

#include "HMLSS_Bfs/Bfs.h"
#include "HMLSS_Bfs/BfsKnobs.h"

#include <vector>

#include "cuda_runtime_api.h"

#include "HMLSS_Graph/Graph.h"

namespace BFS
{
    class BfsCuda : public BfsResult
    {
        public:
            BfsCuda(GpuKnobs knobs, Graph::Graph& graph, unsigned int source);
            virtual ~BfsCuda() override;

            virtual bool kernel() override;
            virtual const std::vector<int>& costs() override;

        private:
            GpuKnobs knobs_;

            unsigned int* edgeOffsetsDevice_;
            unsigned int* edgesDevice_;
            cudaTextureObject_t edgeOffsetsTexture_;
            cudaTextureObject_t edgesTexture_;
            int* costsDevice_;
            bool* doneDevice_;

            std::vector<int> costsHost_;
    };
}

#endif //HMLSS_BFS_CUDA