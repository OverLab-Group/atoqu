
pragma once

include "EmbeddingProvider.hpp"

/
 * @brief Simple deterministic hash-based embedding provider.
 *
 * Useful for offline testing and deterministic behavior.
 */
class HashEmbeddingProvider : public EmbeddingProvider {
public:
    explicit HashEmbeddingProvider(int dim);

    std::vector<float> embed(const std::string& text) override;
    int dimension() const override { return dim_; }

private:
    int dim_;
};
