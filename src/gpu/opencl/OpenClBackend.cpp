
include "OpenClBackend.hpp"

include "Log.hpp"

using atoqu::Log;
using atoqu::LogLevel;

OpenClBackend::OpenClBackend()
    : dimension_(0),
      maxDocsPerBatch_(0),
      ready_(false),
      context_(nullptr),
      queue_(nullptr),
      program_(nullptr),
      kernel_(nullptr) {}

bool OpenClBackend::ensureContext() {
    // v1.2: placeholder for real OpenCL context creation.
    // In a real implementation, you would:
    //  - query platforms/devices
    //  - create context and command queue
    //  - store them in context/queue
    // Here we just mark as "ok" to keep the structure.
    return true;
}

bool OpenClBackend::buildKernel() {
    // v1.2: placeholder for real OpenCL kernel build.
    // You would compile a kernel that computes cosine similarity.
    return true;
}

bool OpenClBackend::initialize(int dimension, std::size_t maxDocsPerBatch) {
    if (dimension <= 0) {
        Log::warn("OpenClBackend::initialize: invalid dimension.");
        return false;
    }
    dimension_ = dimension;
    maxDocsPerBatch_ = maxDocsPerBatch;

    if (!ensureContext()) {
        Log::warn("OpenClBackend::initialize: failed to create OpenCL context.");
        return false;
    }
    if (!buildKernel()) {
        Log::warn("OpenClBackend::initialize: failed to build OpenCL kernel.");
        return false;
    }

    ready_ = true;
    Log::info("OpenClBackend initialized with dimension=" + std::tostring(dimension) +
              ", maxDocsPerBatch=" + std::tostring(maxDocsPerBatch));
    return true;
}

bool OpenClBackend::cosineBatch(const std::vector<float>& query,
                                const std::vector<std::vector<float>>& docs,
                                std::vector<double>& outScores) {
    if (!ready_) {
        Log::warn("OpenClBackend::cosineBatch called before initialize.");
        return false;
    }
    if (query.size() != staticcast<std::sizet>(dimension_)) {
        Log::warn("OpenClBackend::cosineBatch: query dimension mismatch.");
        return false;
    }
    if (docs.empty()) {
        outScores.clear();
        return true;
    }
    if (docs.size() > maxDocsPerBatch_) {
        Log::warn("OpenClBackend::cosineBatch: docs.size() exceeds maxDocsPerBatch, CPU fallback recommended.");
        return false;
    }

    // v1.2: structure is real, but kernel execution is not implemented here.
    // Return false to allow CPU fallback, but keep the API stable.
    return false;
}
