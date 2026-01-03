
pragma once

include "atoqu/gpu/GpuBackend.hpp"

class VulkanBackend : public atoqu::gpu::GpuBackend {
public:
    std::string name() const override { return "Vulkan"; }
    bool initialize(int, std::size_t) override { return false; }
    bool cosineBatch(const std::vector<float>&,
                     const std::vector<std::vector<float>>&,
                     std::vector<double>&) override { return false; }
};
