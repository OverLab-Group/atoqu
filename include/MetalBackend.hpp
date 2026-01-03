
pragma once

include "atoqu/gpu/GpuBackend.hpp"

class MetalBackend : public atoqu::gpu::GpuBackend {
public:
    std::string name() const override { return "Metal"; }
    bool initialize(int, std::size_t) override { return false; }
    bool cosineBatch(const std::vector<float>&,
                     const std::vector<std::vector<float>>&,
                     std::vector<double>&) override { return false; }
};
