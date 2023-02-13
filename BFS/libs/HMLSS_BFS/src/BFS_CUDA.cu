#include "HMLSS_BFS/BFS.h"
#include "HMLSS_BFS/BFSKnobs.h"
#include "HMLSS_BFS/BFS_CUDA.h"

#include <vector>

#include <cuda_runtime.h>

#include "HMLSS_Graph/Graph.h"

namespace BFS
{
    BFSCUDA::BFSCUDA(Graph::Graph& graph, unsigned int source) :
        BFSResult(graph, source)
    {
        cudaMalloc(&edgeOffsetsDevice_, sizeof(unsigned int)*graph.edgeOffsets.capacity());
        cudaMemcpy(edgeOffsetsDevice_, graph.edgeOffsets.data(), sizeof(unsigned int)*graph.edgeOffsets.capacity(), cudaMemcpyKind::cudaMemcpyHostToDevice);

        cudaMalloc(&edgesDevice_, sizeof(unsigned int)*graph.edges.capacity());
        cudaMemcpy(edgesDevice_, graph.edges.data(), sizeof(unsigned int)*graph.edges.capacity(), cudaMemcpyKind::cudaMemcpyHostToDevice);

        cudaMalloc(&costsDevice_, sizeof(int)*graph.nVertices);
        cudaMemset(costsDevice_, -1, sizeof(unsigned int)*graph.nVertices);
        cudaMemset(costsDevice_ + source, 0, sizeof(unsigned int));

        cudaMalloc(&doneDevice_, sizeof(bool));
        cudaMemset(doneDevice_, true, sizeof(bool));
    }

    BFSCUDA::BFSCUDA(BFSResult bfsCPU) :
        BFSCUDA(bfsCPU.graph, bfsCPU.source)
    {}

    BFSCUDA::~BFSCUDA() 
    {
        cudaFree(edgeOffsetsDevice_);
        cudaFree(edgesDevice_);
        cudaFree(costsDevice_);
        cudaFree(doneDevice_);
    }

    namespace
    {
        __global__
        void CUDAkernel(
            unsigned int nVertices,
            unsigned int chunkSize,
            unsigned int *edgeOffsets,
            unsigned int *edges,
            int *costs,
            int currCost,
            bool *done)
        {
            const unsigned int startNode = blockIdx.x*chunkSize;
            const unsigned int stopNode = min(startNode + chunkSize, nVertices);
            for(unsigned int fromNode = startNode; fromNode < stopNode; fromNode++) {
                if(costs[fromNode] == currCost) {
                    const unsigned int nodeEdgesStart = edgeOffsets[fromNode];
                    const unsigned int nodeEdgesEnd = edgeOffsets[fromNode+1];
                    for(unsigned int i = nodeEdgesStart + threadIdx.x; i < nodeEdgesEnd; i+=blockDim.x) {
                        const unsigned int toNode = edges[i];
                        if(costs[toNode] == -1) {
                            costs[toNode] = currCost + 1;
                            *done = false;
                        }
                    }
                }
            }
        }
    }

    bool BFSCUDA::kernel(BFSKnobs::Knobs knobs)
    {
        cudaMemset(doneDevice_, true, sizeof(bool));

        const unsigned int blockDim = knobs.gpuKnobs.blockSize;
        const unsigned int chunkSize = blockDim * knobs.gpuKnobs.chunkFactor;
        const unsigned int gridDim = (graph.nVertices + blockDim - 1)/chunkSize + 1;
        CUDAkernel<<<gridDim, blockDim>>>(graph.nVertices, chunkSize, edgeOffsetsDevice_, edgesDevice_, costsDevice_, currentCost, doneDevice_);
        currentCost++;

        bool done;
        cudaMemcpy(&done, doneDevice_, sizeof(bool), cudaMemcpyKind::cudaMemcpyDeviceToHost);
        return done;
    }

    const std::vector<int>& BFSCUDA::costs() 
    {
        costsHost_.reserve(graph.nVertices);
        costsHost_.resize(costsHost_.capacity());
        cudaMemcpy(costsHost_.data(), costsDevice_, sizeof(int)*graph.nVertices, cudaMemcpyKind::cudaMemcpyDeviceToHost);
        return costsHost_;
    };
}