
Contributor Guide — Atoqu v1.2

Philosophy

- C++17, low-level, dependency-minimal.
- Every file is a legacy piece: readable, documented, testable.
- Prefer small, composable modules (atomic) with clear contracts.
- GPU support is optional but first-class where enabled.

Coding style

- Use #pragma once for headers.
- Namespaces: atoqu for utils, clear module namespaces for core.
- Prefer std::unique_ptr and RAII.
- Avoid exceptions for control flow; use them only at boundaries if needed.
- Keep headers light; avoid heavy includes in public headers.

GPU backends

- CUDA backend:
  - Keep host/device separation clear.
  - Provide CPU fallback if any CUDA call fails.
- OpenCL backend:
  - Build kernels at runtime.
  - Fail closed (CPU fallback) if context or kernel build fails.
- Other backends (Vulkan, Metal, SYCL, HIP):
  - Must be safe stubs or fully implemented; never crash.

Testing

- Add unit tests for new modules (Catch2, CUnit, Unity).
- Add integration tests for new flows and modes.
- Keep perf tests updated for hot paths (VectorStore, AtoquEngine::search).
- Run sanitizers and static analysis locally when possible.

Documentation

- Update Doxygen comments for public APIs.
- Update Sphinx docs when behavior or configuration changes.
- Keep ARCHITECTURE.md and CHANGELOG.md in sync with real behavior.

Workflow

1. Fork and create a feature branch.
2. Implement changes with tests.
3. Run:
   - make debug
   - make test
   - make sanitize
   - make static-analysis
4. Open a pull request with a clear description and references to issues (if any).
