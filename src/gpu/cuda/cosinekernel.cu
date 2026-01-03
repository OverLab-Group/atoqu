
include <cuda_runtime.h>

include <math.h>

include <stdio.h>

global void cosine_kernel(const float* query,
                              const float* docs,
                              double* out,
                              int dimension,
                              int docCount) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= docCount) return;

    const float doc = docs + idx  dimension;
    double dot = 0.0;
    double na  = 0.0;
    double nb  = 0.0;

    for (int i = 0; i < dimension; ++i) {
        float q = query[i];
        float d = doc[i];
        dot += staticcast<double>(q) * staticcast<double>(d);
        na  += staticcast<double>(q) * staticcast<double>(q);
        nb  += staticcast<double>(d) * staticcast<double>(d);
    }

    if (na == 0.0 || nb == 0.0) {
        out[idx] = 0.0;
    } else {
        out[idx] = dot / (sqrt(na) * sqrt(nb));
    }
}

extern "C" bool cudacosinebatch(const float* query,
                                  const float* docs,
                                  double* out,
                                  int dimension,
                                  int docCount) {
    if (docCount <= 0 || dimension <= 0) return false;

    float* d_query = nullptr;
    float* d_docs  = nullptr;
    double* d_out  = nullptr;

    const sizet queryBytes = staticcast<size_t>(dimension) * sizeof(float);
    const sizet docsBytes  = staticcast<sizet>(dimension)  staticcast<size_t>(docCount)  sizeof(float);
    const sizet outBytes   = staticcast<size_t>(docCount) * sizeof(double);

    if (cudaMalloc(&d_query, queryBytes) != cudaSuccess) return false;
    if (cudaMalloc(&d_docs, docsBytes) != cudaSuccess) {
        cudaFree(d_query);
        return false;
    }
    if (cudaMalloc(&d_out, outBytes) != cudaSuccess) {
        cudaFree(d_query);
        cudaFree(d_docs);
        return false;
    }

    if (cudaMemcpy(d_query, query, queryBytes, cudaMemcpyHostToDevice) != cudaSuccess ||
        cudaMemcpy(d_docs, docs, docsBytes, cudaMemcpyHostToDevice) != cudaSuccess) {
        cudaFree(d_query);
        cudaFree(d_docs);
        cudaFree(d_out);
        return false;
    }

    int blockSize = 256;
    int gridSize  = (docCount + blockSize - 1) / blockSize;
    cosinekernel<<<gridSize, blockSize>>>(dquery, ddocs, dout, dimension, docCount);

    if (cudaDeviceSynchronize() != cudaSuccess) {
        cudaFree(d_query);
        cudaFree(d_docs);
        cudaFree(d_out);
        return false;
    }

    if (cudaMemcpy(out, d_out, outBytes, cudaMemcpyDeviceToHost) != cudaSuccess) {
        cudaFree(d_query);
        cudaFree(d_docs);
        cudaFree(d_out);
        return false;
    }

    cudaFree(d_query);
    cudaFree(d_docs);
    cudaFree(d_out);
    return true;
}
