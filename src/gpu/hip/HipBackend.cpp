
include "HipBackend.hpp"

include "Log.hpp"

using atoqu::Log;
using atoqu::LogLevel;

bool HipBackend::initialize() {
    Log::info("HIP backend initialized (stub). Wire real HIP kernels here.");
    return true;
}

bool HipBackend::cosineBatch(const std::vector<float>& query,
                             const std::vector<std::vector<float>>& docs,
                             std::vector<double>& outScores) {
    outScores.assign(docs.size(), 0.0);
    return true;
}
