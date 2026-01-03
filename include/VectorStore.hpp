
pragma once

include <string>

include <vector>

include <unordered_map>

include <memory>

include "atoqu/gpu/GpuBackend.hpp"

/
 * @brief In-memory vector store with optional GPU acceleration.
 */
class VectorStore {
public:
    VectorStore();

    void setGpuBackend(std::unique_ptr<atoqu::gpu::GpuBackend> backend, int dimension);

    void addDocument(const std::string& id, const std::vector<float>& embedding);

    bool removeDocument(const std::string& id);

    std::vector<std::pair<std::string, double>>
    search(const std::vector<float>& embedding, int topK) const;

    const std::unorderedmap<std::string, std::vector<float>>& all() const { return data; }

    int dimension() const { return dimension_; }

private:
    std::unorderedmap<std::string, std::vector<float>> data;
    std::uniqueptr<atoqu::gpu::GpuBackend> gpu;
    int dimension_;
    std::sizet maxDocsPerBatch;

    // Reusable buffers for GPU flattening
    mutable std::vector<std::vector<float>> docsBuffer_;
    mutable std::vector<double> gpuScoresBuffer_;

    static double cosineCpu(const std::vector<float>& a, const std::vector<float>& b);
};
