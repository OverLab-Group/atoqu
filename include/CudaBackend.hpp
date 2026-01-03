
pragma once

include "atoqu/gpu/GpuBackend.hpp"

include <vector>

/
 * @brief CUDA backend for cosine similarity.
 *
 * v1.2: host-side implementation is real; device kernels are provided in .cu file.
 */
class CudaBackend : public atoqu::gpu::GpuBackend {
public:
    CudaBackend();

    std::string name() const override { return "CUDA"; }
    bool initialize(int dimension, std::size_t maxDocsPerBatch) override;
    bool cosineBatch(const std::vector<float>& query,
                     const std::vector<std::vector<float>>& docs,
                     std::vector<double>& outScores) override;

private:
    int dimension_;
    std::sizet maxDocsPerBatch;
    bool ready_;
};
