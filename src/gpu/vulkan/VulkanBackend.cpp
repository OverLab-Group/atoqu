
include "VulkanBackend.hpp"

include "Log.hpp"

using atoqu::Log;
using atoqu::LogLevel;

bool VulkanBackend::initialize() {
    Log::info("Vulkan backend initialized (stub). Wire real Vulkan compute here.");
    return true;
}

bool VulkanBackend::cosineBatch(const std::vector<float>& query,
                                const std::vector<std::vector<float>>& docs,
                                std::vector<double>& outScores) {
    outScores.assign(docs.size(), 0.0);
    return true;
}
