# CONTRIBUTING for AI Agents

This project welcomes autonomous or assisted AI contributions. Please follow this checklist to keep work consistent and reproducible.

## 1) Before You Start
- Read: `README.md`, `README_CLI.md`, `docs/AI_HANDOFF.md`, `docs/LIBRARY_EVALUATION.md`, `docs/SCANNER_EVALUATION.md`.
- Verify build toolchain availability (CMake + compiler). Record environment in `docs/AI_HANDOFF.md`.
- Use the registry pattern for new providers (see `fo/core/registry.hpp`).

## 2) Making Changes
- Prefer small, focused PRs/patches (one feature per change).
- Update or add docs with every code change (what changed, why, how to use).
- For platform-specific code, guard with `#ifdef` and keep portable defaults.
- Avoid reformatting unrelated files; keep diffs minimal.

## 3) Benchmarks & Measurements
- When performance is relevant, add or update benchmarks in `benchmarks/`.
- Record results (hardware, OS, dataset, command, output) in `docs/AI_HANDOFF.md`.
- Report median and p90 over 5 runs; clearly mark estimates vs measured.

## 4) Commit Notes
- Reference files changed and provider names.
- Document any external tools or environment steps needed to reproduce.

## 5) Handoff Updates
- Append a dated section to `docs/AI_HANDOFF.md` titled "Update: <YYYY-MM-DD>" summarizing:
  - What you added/changed
  - How to build/run
  - Issues encountered and how to proceed next

## 6) Style and Conventions
- C++20, warnings enabled.
- Keep public APIs stable; extend instead of breaking when possible.
- Use `std::filesystem::path` for paths internally; convert at boundaries.

Thank you for keeping the project maintainable and friendly to both humans and AI successors.
