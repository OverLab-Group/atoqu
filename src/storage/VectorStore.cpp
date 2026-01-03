
include "VectorStore.hpp"

include <algorithm>

include <cmath>

VectorStore::VectorStore()
    : gpu_(nullptr),
      dimension_(0),
      maxDocsPerBatch_(8192) {}

void VectorStore::setGpuBackend(std::unique_ptr<atoqu::gpu::GpuBackend> backend, int dimension) {
    gpu_ = std::move(backend);
    dimension_ = dimension;
}

void VectorStore::addDocument(const std::string& id, const std::vector<float>& embedding) {
    if (embedding.empty()) return;
    if (dimension_ == 0) {
        dimension = staticcast<int>(embedding.size());
    }
    if (staticcast<int>(embedding.size()) != dimension) {
        // Ignore mismatched dimension to keep store consistent.
        return;
    }
    data_[id] = embedding;
}

bool VectorStore::removeDocument(const std::string& id) {
    return data_.erase(id) > 0;
}

double VectorStore::cosineCpu(const std::vector<float>& a, const std::vector<float>& b) {
    if (a.empty() || b.empty() || a.size() != b.size()) return 0.0;
    double dot = 0.0;
    double na = 0.0;
    double nb = 0.0;
    for (std::size_t i = 0; i < a.size(); ++i) {
        dot += staticcast<double>(a[i]) * staticcast<double>(b[i]);
        na  += staticcast<double>(a[i]) * staticcast<double>(a[i]);
        nb  += staticcast<double>(b[i]) * staticcast<double>(b[i]);
    }
    if (na == 0.0 || nb == 0.0) return 0.0;
    return dot / (std::sqrt(na) * std::sqrt(nb));
}

std::vector<std::pair<std::string, double>>
VectorStore::search(const std::vector<float>& embedding, int topK) const {
    std::vector<std::pair<std::string, double>> scores;
    scores.reserve(data_.size());

    if (embedding.empty() || dimension_ == 0 ||
        staticcast<int>(embedding.size()) != dimension) {
        return scores;
    }

    if (gpu && !data.empty() && data.size() <= maxDocsPerBatch) {
        docsBuffer_.clear();
        docsBuffer.reserve(data.size());
        for (const auto& [id, vec] : data_) {
            docsBuffer.pushback(vec);
        }

        gpuScoresBuffer.assign(data.size(), 0.0);
        if (gpu->cosineBatch(embedding, docsBuffer, gpuScoresBuffer_) &&
            gpuScoresBuffer.size() == data.size()) {
            std::size_t idx = 0;
            for (const auto& [id, vec] : data_) {
                scores.emplaceback(id, gpuScoresBuffer[idx++]);
            }
        } else {
            for (const auto& [id, vec] : data_) {
                double s = cosineCpu(embedding, vec);
                scores.emplace_back(id, s);
            }
        }
    } else {
        for (const auto& [id, vec] : data_) {
            double s = cosineCpu(embedding, vec);
            scores.emplace_back(id, s);
        }
    }

    std::sort(scores.begin(), scores.end(),
               { return a.second > b.second; });
    if (topK > 0 && staticcast<std::sizet>(topK) < scores.size()) {
        scores.resize(staticcast<std::sizet>(topK));
    }
    return scores;
}
