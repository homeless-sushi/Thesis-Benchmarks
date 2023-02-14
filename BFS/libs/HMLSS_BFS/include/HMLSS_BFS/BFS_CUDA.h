#ifndef HMLSS_BFS_CUDA
#define HMLSS_BFS_CUDA

#include "HMLSS_BFS/BFS.h"
#include "HMLSS_BFS/BFSKnobs.h"

#include <vector>

#include "HMLSS_Graph/Graph.h"

namespace BFS
{
    struct BFSCUDA : public BFSResult
    {
            BFSCUDA(Graph::Graph& graph, unsigned int source);
            BFSCUDA(BFSResult bfsCPU);
            virtual ~BFSCUDA() override;
            virtual bool kernel(BFSKnobs::Knobs knobs) override;
            const std::vector<int>& costs();

        private:
            unsigned int* edgeOffsetsDevice_;
            unsigned int* edgesDevice_;
            int* costsDevice_;
            bool* doneDevice_;

            std::vector<int> costsHost_;
    };
}

#endif //HMLSS_BFS_CUDA