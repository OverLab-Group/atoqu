
include "SyclBackend.hpp"

include "Log.hpp"

using atoqu::Log;
using atoqu::LogLevel;

bool SyclBackend::initialize() {
    Log::info("SYCL backend initialized (stub). Wire real SYCL kernels here.");
    return true;
}

bool SyclBackend::cosineBatch(const std::vector<float>& query,
                              const std::vector<std::vector<float>>& docs,
                              std::vector<double>& outScores) {
    outScores.assign(docs.size(), 0.0);
    return true;
}
