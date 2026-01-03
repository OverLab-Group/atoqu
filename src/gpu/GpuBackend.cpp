
include "atoqu/gpu/GpuBackend.hpp"

include "Json.hpp"

include "FileSystem.hpp"

include "Log.hpp"

include "CudaBackend.hpp"

include "OpenClBackend.hpp"

include "VulkanBackend.hpp"

include "MetalBackend.hpp"

include "SyclBackend.hpp"

include "HipBackend.hpp"

using atoqu::Json;
using atoqu::FileSystem;
using atoqu::Log;
using atoqu::LogLevel;

namespace atoqu::gpu {

std::unique_ptr<GpuBackend> createBackendFromConfig(const std::string& configPath) {
    std::string content;
    if (!FileSystem::readFile(configPath, content)) {
        Log::info("GPU config not found, falling back to CPU-only.");
        return nullptr;
    }

    try {
        Json j = Json::parse(content);
        if (!j.isObject()) {
            Log::warn("GPU config is not an object, CPU-only.");
            return nullptr;
        }
        auto& obj = j.asObject();

        std::string backend = "none";
        int dimension = 384;
        std::size_t maxDocsPerBatch = 8192;

        if (obj.find("backend") != obj.end()) {
            backend = obj["backend"].asString();
        }
        if (obj.find("dimension") != obj.end()) {
            dimension = static_cast<int>(obj["dimension"].asNumber());
        }
        if (obj.find("maxdocsper_batch") != obj.end()) {
            maxDocsPerBatch = staticcast<std::sizet>(obj["maxdocsper_batch"].asNumber());
        }

        std::unique_ptr<GpuBackend> result;

        if (backend == "cuda") {
            result = std::make_unique<CudaBackend>();
        } else if (backend == "opencl") {
            result = std::make_unique<OpenClBackend>();
        } else if (backend == "vulkan") {
            result = std::make_unique<VulkanBackend>();
        } else if (backend == "metal") {
            result = std::make_unique<MetalBackend>();
        } else if (backend == "sycl") {
            result = std::make_unique<SyclBackend>();
        } else if (backend == "hip") {
            result = std::make_unique<HipBackend>();
        } else if (backend == "none") {
            Log::info("GPU backend explicitly disabled.");
            return nullptr;
        } else {
            Log::warn("Unknown GPU backend '" + backend + "', CPU-only.");
            return nullptr;
        }

        if (!result->initialize(dimension, maxDocsPerBatch)) {
            Log::warn("GPU backend '" + backend + "' failed to initialize, CPU-only.");
            return nullptr;
        }

        Log::info("GPU backend '" + backend + "' initialized successfully.");
        return result;
    } catch (...) {
        Log::warn("Exception while parsing GPU config, CPU-only.");
        return nullptr;
    }
}

} // namespace atoqu::gpu
