Architecture
============

- Core engine: AtoquEngine
- Stores:
  - DocumentStore
  - VectorStore (CPU + optional GPU)
- Modes:
  - NormalMode
  - LiteralMode
  - VectorMode
  - HybridMode
  - BM25Mode
  - RecencyMode
  - TagBoostMode
- Embeddings:
  - HashEmbeddingProvider
  - LlmEmbeddingProvider
- GPU backends:
  - CUDA (production-ready cosine)
  - OpenCL (host-side skeleton)
  - Vulkan/Metal/SYCL/HIP (safe stubs)
