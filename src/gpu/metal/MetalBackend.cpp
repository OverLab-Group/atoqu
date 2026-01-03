
include "MetalBackend.hpp"

include "Log.hpp"

using atoqu::Log;
using atoqu::LogLevel;

bool MetalBackend::initialize() {
    Log::info("Metal backend initialized (stub). Wire real Metal kernels here.");
    return true;
}

bool MetalBackend::cosineBatch(const std::vector<float>& query,
                               const std::vector<std::vector<float>>& docs,
                               std::vector<double>& outScores) {
    outScores.assign(docs.size(), 0.0);
    return true;
}
