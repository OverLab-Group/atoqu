Testing
=======

Atoqu v1.2 includes a layered test strategy:

- Unit tests:
  - VectorStore (CPU + GPU-aware)
  - Modes (BM25, etc.)
- Integration tests:
  - Multi-mode search via AtoquEngine
- Security tests:
  - Sanitizer-friendly smoke tests
- Performance tests:
  - CPU vs GPU vector search benchmarks

Run tests:

.. code-block:: bash

   mkdir -p build
   cd build
   cmake ..
   cmake --build .
   ctest --output-on-failure
