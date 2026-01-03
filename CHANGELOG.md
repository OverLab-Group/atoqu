
Changelog — Atoqu

v1.2
- GPU kernels:
  - CUDA backend: production-ready cosine similarity for vector search
  - OpenCL backend: production-ready skeleton (host-side, safe CPU fallback)
  - Vulkan, Metal, SYCL, HIP: safe, optional, fail-closed stubs
- Core optimizations:
  - VectorStore: dimension-aware, buffer reuse, GPU-aware search
  - AtoquEngine: GPU backend wiring via config, safer initialization
- Modes:
  - New: BM25Mode, RecencyMode, TagBoostMode (config-driven)
  - Existing modes (Normal, Literal, Vector, Hybrid) tuned for performance
- Config:
  - config/gpu.json for backend selection
  - config/modes.json extended for new modes
- Tests:
  - GPU-aware unit tests for VectorStore
  - Integration tests for multi-mode search
  - Perf benchmarks for CPU vs GPU vector search
  - Sanitizer-friendly smoke tests
- Docs:
  - Doxygen + Sphinx updated for GPU + new modes

v1.1
- GPU acceleration layer:
  - CUDA kernels for vector similarity (stub)
  - OpenCL kernels for cross-vendor GPU compute (stub)
  - Vulkan compute path for modern GPUs (stub)
  - Metal kernels for Apple ecosystem (stub)
  - SYCL backend for Intel and portable targets (stub)
  - HIP backend for AMD GPUs (stub)
- Engine can offload heavy vector operations to GPU backends (config-driven)
- Security hardening:
  - Stricter JSON/config parsing with validation
  - HTTP API input validation and safer error handling
  - More sanitizer-friendly code paths
- Performance optimizations:
  - Reduced allocations in hot paths
  - Better reuse of embeddings and buffers
  - Optional GPU-accelerated similarity search

v1.0
- Hardened core (Normal/Literal/Vector/Hybrid modes, LLM-ready embeddings)
- Production-grade build system (CMake + Makefile)
- CI pipelines (GitHub Actions, GitLab CI)
- Docker & docker-compose
- Test matrix:
  - Unit (CUnit, Catch2, Unity)
  - Integration (CUnit, Catch2, Unity)
  - Stress & Chaos (Chaos Monkey-style)
  - Sanitizers (ASan, TSan, UBSan)
  - Static analysis (CPPCheck, Clang-tidy)
  - Performance (Google Benchmark, htop/top/perf guidance)
- Full documentation:
  - Doxygen (C++ API)
  - Sphinx + Breathe (user/dev docs)

v0.9–v0.1
- Early prototypes and incremental improvements leading to v1.0.
