
pragma once

include "atoqu/gpu/GpuBackend.hpp"

include <vector>

include <string>

/
 * @brief OpenCL backend for cosine similarity.
 *
 * v1.2: host-side implementation is real; kernel is compiled at runtime.
 */
class OpenClBackend : public atoqu::gpu::GpuBackend {
public:
    OpenClBackend();

    std::string name() const override { return "OpenCL"; }
    bool initialize(int dimension, std::size_t maxDocsPerBatch) override;
    bool cosineBatch(const std::vector<float>& query,
                     const std::vector<std::vector<float>>& docs,
                     std::vector<double>& outScores) override;

private:
    int dimension_;
    std::sizet maxDocsPerBatch;
    bool ready_;

    // Minimal OpenCL handles (opaque pointers to avoid heavy headers here).
    void* context_;
    void* queue_;
    void* program_;
    void* kernel_;

    bool buildKernel();
    bool ensureContext();
};
