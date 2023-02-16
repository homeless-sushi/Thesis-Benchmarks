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
        cudaMalloc(&edgeOffsetsDevice_, sizeof(unsigned int)*graph.edgeOffsets.size());
        cudaMemcpy(edgeOffsetsDevice_, graph.edgeOffsets.data(), sizeof(unsigned int)*graph.edgeOffsets.size(), cudaMemcpyKind::cudaMemcpyHostToDevice);

        cudaMalloc(&edgesDevice_, sizeof(unsigned int)*graph.edges.size());
        cudaMemcpy(edgesDevice_, graph.edges.data(), sizeof(unsigned int)*graph.edges.size(), cudaMemcpyKind::cudaMemcpyHostToDevice);

        memset(&edgeOffsetsTexture_, 0, sizeof(cudaTextureObject_t));
        memset(&edgesTexture_, 0, sizeof(cudaTextureObject_t));

        cudaMalloc(&costsDevice_, sizeof(int)*graph.nVertices);
        cudaMemset(costsDevice_, -1, sizeof(unsigned int)*graph.nVertices);
        cudaMemset(costsDevice_ + source, 0, sizeof(unsigned int));

        cudaMalloc(&doneDevice_, sizeof(bool));
        cudaMemset(doneDevice_, true, sizeof(bool));
    }

    BFSCUDA::BFSCUDA(
        Graph::Graph& graph,
        unsigned int source,
        int currentCost,
        std::vector<int> costs) :
        BFSResult(graph, source)
    {
        this->currentCost = currentCost;

        cudaMalloc(&edgeOffsetsDevice_, sizeof(unsigned int)*graph.edgeOffsets.size());
        cudaMemcpy(edgeOffsetsDevice_, graph.edgeOffsets.data(), sizeof(unsigned int)*graph.edgeOffsets.size(), cudaMemcpyKind::cudaMemcpyHostToDevice);

        cudaMalloc(&edgesDevice_, sizeof(unsigned int)*graph.edges.size());
        cudaMemcpy(edgesDevice_, graph.edges.data(), sizeof(unsigned int)*graph.edges.size(), cudaMemcpyKind::cudaMemcpyHostToDevice);

        memset(&edgeOffsetsTexture_, 0, sizeof(cudaTextureObject_t));
        memset(&edgesTexture_, 0, sizeof(cudaTextureObject_t));

        cudaMalloc(&costsDevice_, sizeof(int)*costs.size());
        cudaMemcpy(costsDevice_, costs.data(), sizeof(int)*costs.size(), cudaMemcpyKind::cudaMemcpyHostToDevice);

        cudaMalloc(&doneDevice_, sizeof(bool));
        cudaMemset(doneDevice_, true, sizeof(bool));
    }

    BFSCUDA::BFSCUDA(BFSResult bfsCPU) :
        BFSCUDA(bfsCPU.graph, bfsCPU.source, bfsCPU.currentCost, bfsCPU.costs())
    {}

    BFSCUDA::~BFSCUDA() 
    {
        cudaFree(edgeOffsetsDevice_);
        cudaFree(edgesDevice_);
        cudaDestroyTextureObject(edgeOffsetsTexture_);
        cudaDestroyTextureObject(edgesTexture_);
        cudaFree(costsDevice_);
        cudaFree(doneDevice_);
    }

    namespace
    {
        void createTextureObject(unsigned int* src, cudaTextureObject_t* dst, size_t size)
        {
            cudaResourceDesc resourceDesc;
            memset(&resourceDesc, 0, sizeof(resourceDesc));
            resourceDesc.resType = cudaResourceTypeLinear;
            resourceDesc.res.linear.devPtr = src;
            resourceDesc.res.linear.desc.f = cudaChannelFormatKindUnsigned;
            resourceDesc.res.linear.desc.x = sizeof(unsigned int)*CHAR_BIT;
            resourceDesc.res.linear.sizeInBytes = sizeof(unsigned int)*size;
            cudaTextureDesc texDesc;
            memset(&texDesc, 0, sizeof(texDesc));
            texDesc.readMode = cudaReadModeElementType;
            cudaCreateTextureObject(dst, &resourceDesc, &texDesc, NULL);
        }

        __global__
        void CUDAkernel(
            unsigned int nVertices,
            unsigned int chunkSize,
            unsigned int *edgeOffsetsDevice,
            unsigned int *edgesDevice,
            cudaTextureObject_t edgeOffsetsTexture,
            cudaTextureObject_t edgesTexture,
            BFSKnobs::GPUKnobs::MEMORY_TYPE edgeOffsetsType,
            BFSKnobs::GPUKnobs::MEMORY_TYPE edgesType,
            int *costs,
            int currCost,
            bool *done)
        {
            const unsigned int startNode = blockIdx.x*chunkSize;
            const unsigned int stopNode = min(startNode + chunkSize, nVertices);
            for(unsigned int fromNode = startNode; fromNode < stopNode; fromNode++) {
                if(costs[fromNode] == currCost) {
                    unsigned int nodeEdgesStart;
                    unsigned int nodeEdgesEnd;
                    switch (edgeOffsetsType)
                    {
                        case BFSKnobs::GPUKnobs::MEMORY_TYPE::TEXTURE_MEMORY:
                            nodeEdgesStart = tex1Dfetch<unsigned int>(edgeOffsetsTexture, fromNode);
                            nodeEdgesEnd = tex1Dfetch<unsigned int>(edgeOffsetsTexture, fromNode+1);
                            break;
                        
                        default:
                            nodeEdgesStart = edgeOffsetsDevice[fromNode];
                            nodeEdgesEnd = edgeOffsetsDevice[fromNode+1];
                            break;
                    }
                    for(unsigned int i = nodeEdgesStart + threadIdx.x; i < nodeEdgesEnd; i+=blockDim.x) {
                        unsigned int toNode;
                        switch (edgesType)
                        {
                            case BFSKnobs::GPUKnobs::MEMORY_TYPE::TEXTURE_MEMORY:
                                toNode = tex1Dfetch<unsigned int>(edgesTexture, i);
                                break;
                            
                            default:
                                toNode = edgesDevice[i];
                                break;
                        }
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

        if(knobs.gpuKnobs.edgeOffsets == BFSKnobs::GPUKnobs::MEMORY_TYPE::TEXTURE_MEMORY && edgeOffsetsTexture_ == 0)
            createTextureObject(edgeOffsetsDevice_, &edgeOffsetsTexture_, graph.edgeOffsets.size());
        if(knobs.gpuKnobs.edges == BFSKnobs::GPUKnobs::MEMORY_TYPE::TEXTURE_MEMORY && edgesTexture_ == 0)
            createTextureObject(edgesDevice_, &edgesTexture_, graph.edges.size());

        const unsigned int blockDim = knobs.gpuKnobs.blockSize;
        const unsigned int chunkSize = blockDim * knobs.gpuKnobs.chunkFactor;
        const unsigned int gridDim = (graph.nVertices + blockDim - 1)/chunkSize + 1;

        CUDAkernel<<<gridDim, blockDim>>>(
            graph.nVertices, 
            chunkSize,
            edgeOffsetsDevice_, 
            edgesDevice_,
            edgeOffsetsTexture_,
            edgesTexture_,
            knobs.gpuKnobs.edgeOffsets,
            knobs.gpuKnobs.edges,
            costsDevice_,
            currentCost,
            doneDevice_);
        
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