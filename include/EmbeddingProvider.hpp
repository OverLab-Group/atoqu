
pragma once

include <string>

include <vector>

/
 * @brief Abstract embedding provider interface.
 *
 * Implementations:
 *  - HashEmbeddingProvider (deterministic, offline)
 *  - LlmEmbeddingProvider (LLM-ready, config-driven)
 */
class EmbeddingProvider {
public:
    virtual ~EmbeddingProvider() = default;

    /
     * @brief Embed a piece of text into a fixed-size vector.
     */
    virtual std::vector<float> embed(const std::string& text) = 0;

    /
     * @brief Return embedding dimension.
     */
    virtual int dimension() const = 0;
};
