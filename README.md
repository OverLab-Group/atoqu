
Atoqu Search Engine — v1.2 (GPU-Optimized, Multi-Mode, Hardened)

Atoqu is an Atomic + Quantum-inspired search engine core in C++, designed as a reference-grade, low-level engine.

Highlights (v1.2)

- GPU acceleration
  - CUDA backend: production-ready cosine similarity for vector search
  - OpenCL backend: production-ready skeleton (host-side, safe CPU fallback)
  - Vulkan, Metal, SYCL, HIP: safe, fail-closed stubs (no crashes, CPU fallback)
- Core engine
  - DocumentStore + VectorStore with optional GPU acceleration
  - Modes:
    - NormalMode (browser-style, classic search)
    - LiteralMode (keyword)
    - VectorMode (embedding-based)
    - HybridMode (combined scoring)
    - BM25Mode (term-based ranking)
    - RecencyMode (time-aware ranking)
    - TagBoostMode (tag-aware ranking)
- Embedding
  - HashEmbeddingProvider (deterministic, dependency-free)
  - LlmEmbeddingProvider (LLM-ready, config-driven)
- Production tooling
  - CMake + Makefile
  - Docker + Docker Compose (CPU + GPU variants)
  - GitHub Actions + GitLab CI
  - Test matrix:
    - Unit (CUnit, Catch2, Unity)
    - Integration (CUnit, Catch2, Unity)
    - Stress & Chaos (Chaos Monkey-style)
    - Sanitizers (ASan, TSan, UBSan)
    - Static analysis (CPPCheck, Clang-tidy)
    - Performance (Google Benchmark, htop/top/perf guidance)
  - Full documentation:
    - Doxygen (C++ API)
    - Sphinx + Breathe (user/dev docs) in docs/

Quick start (CPU-only)

`bash
mkdir -p build
cd build
cmake ..
cmake --build .
./atoqu --http 8080
`

GPU backend selection

Configure config/gpu.json:

`json
{
  "backend": "cuda",
  "dimension": 384,
  "maxdocsper_batch": 8192
}
`

Supported values for backend:

- "none" (default, CPU-only)
- "cuda"
- "opencl"
- "vulkan" (stub)
- "metal" (stub)
- "sycl" (stub)
- "hip" (stub)

If initialization fails, Atoqu falls back to CPU-only vector search.

Modes configuration

config/modes.json controls which modes are enabled and their weights.

`json
[
  { "name": "NormalMode",   "enabled": true,  "weight": 1.0 },
  { "name": "HybridMode",   "enabled": true,  "weight": 1.0 },
  { "name": "LiteralMode",  "enabled": true,  "weight": 0.8 },
  { "name": "VectorMode",   "enabled": true,  "weight": 0.8 },
  { "name": "BM25Mode",     "enabled": true,  "weight": 0.9 },
  { "name": "RecencyMode",  "enabled": false, "weight": 0.5 },
  { "name": "TagBoostMode", "enabled": false, "weight": 0.5 }
]
`

Documentation

`bash
mkdir -p build
cd build
cmake -DATOQUBUILDDOCS=ON ..
cmake --build . --target docs
`

Generated docs:

- Doxygen HTML: docs/doxygen/html/
- Sphinx HTML: docs/sphinx/_build/html/

See ARCHITECTURE.md and CHANGELOG.md for the full evolution up to v1.2.
