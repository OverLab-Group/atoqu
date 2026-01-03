
pragma once

include "EmbeddingProvider.hpp"

include "Json.hpp"

include "FileSystem.hpp"

include <string>

/
 * @brief LLM-ready embedding provider (config-driven).
 *
 * v0.9: still offline, but structured to be easily wired to a real LLM service.
 */
class LlmEmbeddingProvider : public EmbeddingProvider {
public:
    explicit LlmEmbeddingProvider(const std::string& configPath);

    std::vector<float> embed(const std::string& text) override;
    int dimension() const override { return dim_; }

private:
    std::string modelName_;
    int dim_;
    double scale_;
};
