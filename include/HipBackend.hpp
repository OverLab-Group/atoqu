
pragma once

include "atoqu/gpu/GpuBackend.hpp"

class HipBackend : public atoqu::gpu::GpuBackend {
public:
    std::string name() const override { return "HIP"; }
    bool initialize(int, std::size_t) override { return false; }
    bool cosineBatch(const std::vector<float>&,
                     const std::vector<std::vector<float>>&,
                     std::vector<double>&) override { return false; }
};
