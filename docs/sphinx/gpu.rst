GPU Acceleration
================

Backends
--------

- CUDA: production-ready cosine similarity for vector search.
- OpenCL: host-side skeleton, safe CPU fallback.
- Vulkan, Metal, SYCL, HIP: safe stubs (no crashes, CPU fallback).

Configuration
-------------

config/gpu.json:

.. code-block:: json

   {
     "backend": "cuda",
     "dimension": 384,
     "maxdocsper_batch": 8192
   }

If initialization fails, Atoqu falls back to CPU-only vector search.
