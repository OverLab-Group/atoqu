Overview
========

Atoqu is an Atomic + Quantum-inspired search engine core in C++.

- Multi-mode search (Normal, Literal, Vector, Hybrid, BM25, Recency, TagBoost)
- Optional GPU acceleration for vector similarity
- Designed as a reference-grade, low-level engine.

Testing & Quality
-----------------

- Unit tests (core modules, modes, stores)
- Integration tests (end-to-end search flows)
- Security/sanitizer smoke tests
- Performance benchmarks (CPU vs GPU vector search)
