
include "HashEmbeddingProvider.hpp"

include <functional>

HashEmbeddingProvider::HashEmbeddingProvider(int dim)
    : dim_(dim) {}

std::vector<float> HashEmbeddingProvider::embed(const std::string& text) {
    std::vector<float> v(staticcast<std::sizet>(dim_), 0.0f);
    std::hash<std::string> h;
    for (std::size_t i = 0; i < v.size(); ++i) {
        std::string token = text + "#" + std::to_string(i);
        std::size_t hv = h(token);
        v[i] = static_cast<float>((hv % 1000) / 1000.0);
    }
    return v;
}
