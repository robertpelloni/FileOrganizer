# Release Notes - v2.1.0

**Date:** 2025-12-31
**Codename:** "Clean Sweep"

## Summary
Version 2.1.0 is a major maintenance and infrastructure release. We have synchronized over 130 external dependencies (submodules) to their latest stable versions, ensuring security patches and performance improvements are incorporated. Additionally, we have solidified the testing infrastructure with new Fuzz Testing targets and comprehensive benchmarks.

## 🏁 Key Highlights

### 📦 Dependency Overhaul
- **Submodule Sync:** Updated all `libs/` submodules to their latest upstream commits.
- **Libraries Updated:** FFmpeg, ImageMagick, OpenCV, boost, fmt, spdlog, and 125+ others.
- **Build System:** Improved CMake configuration to handle new dependency versions.

### ⚡ Performance Benchmarks
We have established a formal benchmarking baseline (`docs/BENCHMARKS.md`):
- **Scanning:** Confirmed `std::filesystem` (~2.5ms) outperforms legacy Win32 APIs (~2.9ms) for standard workloads.
- **Hashing:** Validated `xxHash64` (2.77 GiB/s) is **6.6x faster** than `BLAKE3` (426 MiB/s), making it the ideal default for change detection.

### 🛡️ Robustness & Fuzzing
- **Fuzz Testing:** Added `fuzz_metadata` and `fuzz_rule_engine` targets.
- **Infrastructure:** Integrated Google Benchmark for performance tracking.
- **CI/CD:** Added automated Submodule Dashboard generation.

### 📚 Documentation
- New **Submodule Dashboard** (`docs/SUBMODULES.md`).
- New **Benchmark Report** (`docs/BENCHMARKS.md`).
- Updated **Developer Guide** with Fuzzing instructions.

## 🛠️ Fixes & Improvements
- **Build:** Fixed compilation issues in `tests/fuzz` related to missing headers.
- **Packaging:** Added MSI installer generation scripts (`scripts/package_msi.bat`).
- **Docs:** Refined `ROADMAP.md` and `PROJECT_STRUCTURE.md`.

## Known Issues
- MSI Installer generation requires manual installation of WiX Toolset v3.11+.

## Contributors
- @opencode (Maintainer)
