
Security Policy — Atoqu v1.2

Threat model

- Atoqu may run as a network-facing service.
- Attack surface:
  - HTTP API
  - Plugin loading
  - Config parsing
  - File I/O
  - GPU backends (device selection, kernel execution)

v1.2 Hardening

- Stricter JSON/config parsing with validation and safe fallbacks.
- HTTP API:
  - Validate methods, paths, and payload sizes.
  - Avoid throwing exceptions across API boundaries.
- GPU backends:
  - Fail closed (CPU-only) if initialization fails.
  - No unvalidated device selection from user input.
  - No direct user-controlled kernel parameters.

Recommendations

- Run behind a reverse proxy (nginx, Caddy, etc.).
- Use non-root user inside containers.
- Enable ASan/TSan/UBSan builds in CI for new changes.
- Avoid logging sensitive data (queries, user identifiers).
- Keep dependencies minimal and pinned.
- Regularly run static analysis (CPPCheck, Clang-tidy).

Reporting vulnerabilities

- Prefer private, coordinated disclosure.
- Provide:
  - Affected version
  - Reproduction steps
  - Impact assessment
