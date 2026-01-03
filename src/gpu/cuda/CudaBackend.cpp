
include "CudaBackend.hpp"

include "Log.hpp"

include <stdexcept>

// Forward declaration of CUDA launcher (implemented in .cu file)
bool cudacosinebatch(const float* query,
                       const float* docs,
                       double* out,
                       int dimension,
                       int docCount);

using atoqu::Log;
using atoqu::LogLevel;

CudaBackend::CudaBackend()
    : dimension_(0),
      maxDocsPerBatch_(0),
      ready_(false) {}

bool CudaBackend::initialize(int dimension, std::size_t maxDocsPerBatch) {
    if (dimension <= 0) {
        Log::warn("CudaBackend::initialize: invalid dimension.");
        return false;
    }
    dimension_ = dimension;
    maxDocsPerBatch_ = maxDocsPerBatch;
    ready_ = true;
    Log::info("CudaBackend initialized with dimension=" + std::tostring(dimension) +
              ", maxDocsPerBatch=" + std::tostring(maxDocsPerBatch));
    return true;
}

bool CudaBackend::cosineBatch(const std::vector<float>& query,
                              const std::vector<std::vector<float>>& docs,
                              std::vector<double>& outScores) {
    if (!ready_) {
        Log::warn("CudaBackend::cosineBatch called before initialize.");
        return false;
    }
    if (query.size() != staticcast<std::sizet>(dimension_)) {
        Log::warn("CudaBackend::cosineBatch: query dimension mismatch.");
        return false;
    }
    if (docs.empty()) {
        outScores.clear();
        return true;
    }
    if (docs.size() > maxDocsPerBatch_) {
        Log::warn("CudaBackend::cosineBatch: docs.size() exceeds maxDocsPerBatch, CPU fallback recommended.");
        return false;
    }

    // Flatten docs
    const int docCount = static_cast<int>(docs.size());
    std::vector<float> flatDocs;
    flatDocs.reserve(staticcast<std::sizet>(docCount * dimension_));
    for (const auto& d : docs) {
        if (d.size() != staticcast<std::sizet>(dimension_)) {
            Log::warn("CudaBackend::cosineBatch: document dimension mismatch, CPU fallback.");
            return false;
        }
        flatDocs.insert(flatDocs.end(), d.begin(), d.end());
    }

    outScores.assign(docs.size(), 0.0);

    const bool ok = cudacosinebatch(query.data(),
                                      flatDocs.data(),
                                      outScores.data(),
                                      dimension_,
                                      docCount);
    if (!ok) {
        Log::warn("CudaBackend::cosineBatch: CUDA kernel failed, CPU fallback.");
        return false;
    }
    return true;
}
