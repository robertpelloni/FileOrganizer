# AGENTS.md

This document provides guidance for AI agents contributing to the FileOrganizer project.

## 1. Guiding Principles

- **Follow the Roadmap:** The `docs/ROADMAP.md` provides the overall direction for the project.
- **Small, Focused Changes:** Prefer small, incremental changes that are easy to review.
- **Document Everything:** Every change should be accompanied by relevant documentation updates.
- **Benchmark Performance:** When performance is relevant, add or update benchmarks.

## 2. Getting Started

- **Read the Documentation:** Before making any changes, familiarize yourself with the project by reading:
    - `README.md`
    - `README_CLI.md`
    - `docs/ROADMAP.md`
    - `docs/LIBRARY_EVALUATION.md`
    - `docs/SCANNER_EVALUATION.md`
    - `docs/DATABASE_SCHEMA.md`
    - `docs/BENCHMARKING_PLAN.md`
- **Set up the Build Environment:**
    - Ensure you have a C++20 compiler and CMake installed.
    - For optional dependencies, use vcpkg.
    - See `README.md` for detailed build instructions.

## 3. Making Changes

- **Use the Registry Pattern:** For new providers (scanners, hashers, etc.), use the registry pattern in `fo/core/registry.hpp`.
- **Platform-Specific Code:** Guard platform-specific code with `#ifdef` and provide portable defaults.
- **Minimize Diff Noise:** Avoid reformatting unrelated files.

## 4. Handoff Log

This section serves as a running log of AI agent activity. When you have completed a task, please add a new entry to this log.

---

### Update: 2025-11-14

**Author:** GitHub Copilot (engine: GPT-5)

**Scope:** FileOrganizer — scanning providers (first plugin choice), build environment status, and next steps.

**Current Status:**

- Project structure: CMake (root), `core/` static lib, `cli/` executable.
- Provider registry pattern in place (`fo/core/registry.hpp`).
- Scanners implemented:
  - `std` — C++20 `std::filesystem` recursive scanner.
    - File: `core/src/scanner_std.cpp`
  - `win32` — Native Windows API (`FindFirstFileExW`).
    - File: `core/src/scanner_win32.cpp`
  - `dirent` — POSIX-style `opendir/readdir` + `stat` (Windows shim in `libs/dirent`).
    - File: `core/src/scanner_dirent.cpp`
- Docs updated:
  - `docs/SCANNER_EVALUATION.md` — comparative analysis + recommendations
  - `README_CLI.md` — `--scanner=std|win32|dirent` usage
  - `docs/LIBRARY_EVALUATION.md` — cross-linked scanner section
  - `benchmarks/` — `fo_bench_scanner` simple throughput benchmark
  - `docs/BENCHMARKING_RUNBOOK.md` — how to build and run benchmark

**Findings (Windows):**

- Win32 enumeration is the fastest and most complete on NTFS; use `win32` as default on Windows.
- `dirent` is almost a tie in raw enumeration speed when only basic metadata is needed.
- `std::filesystem` is portable and safe but typically slower on Windows due to abstraction overhead.
- We currently fetch `mtime` using `std::filesystem::last_write_time` even in the Win32/dirent scanners for consistency; this can be optimized later by converting native times to `std::chrono::file_clock`.

**Build Environment Notes:**

Observed on this machine:
- CMake present at `C:\Program Files\CMake\bin\cmake.exe`.
- Visual Studio 2022 generator not detected (CMake error: no VS instance).
- Ninja available via CLion (`.../bin/ninja/win/x64/ninja.exe`).
- CMake configure with Ninja failed due to missing compiler in PATH (`No CMAKE_CXX_COMPILER could be found`).

Bench build note:
- The benchmark target `fo_bench_scanner` builds with the same toolchain as the CLI. Follow the runbook for commands.

Action for future runs:
- Install and use one of:
  1) Visual Studio 2022 (or Build Tools). Then:
     - Configure: `cmake -S . -B build -G "Visual Studio 17 2022" -A x64`
     - Build: `cmake --build build --config Release`
  2) LLVM/Clang or MSVC in Developer Prompt, with Ninja:
     - Configure: `cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release`
     - Build: `cmake --build build`
- Alternatively, open the existing `OpenFileOrganizer.sln` for legacy GUI; CLI is separate.

**Implementation Pointers:**

- Interface: `IFileScanner` — see `core/include/fo/core/interfaces.hpp`.
- Types: `FileInfo` — path, size, mtime in `core/include/fo/core/types.hpp`.
- Registration: static lambda with `Registry<IFileScanner>::instance().add("name", factory)`.
- Extension filter: all scanners normalize extensions to lowercase and accept both `.jpg` and `jpg` forms.
- Symlink policy: not followed by default (flag exists in interface; `follow_symlinks` currently used in `std` scanner only).

**Known Gaps / TODOs:**

- Performance: Win32 scanner currently calls `std::filesystem::last_write_time` per file; replace with FILETIME→`file_clock` conversion for speed.
- Metadata coverage: Expose Windows attributes, reparse tags, file IDs when available (extend `FileInfo` optionally on Windows).
- Benchmarks: Implement Google Benchmark harness for scanners (`BM_Scanner_*`).
- CLI: Expose provider enumeration (`--list-providers`) via registry.
- CI: Add GitHub Actions and vcpkg manifest (Exiv2/BLAKE3/Tesseract) later.

**Next Concrete Steps (Suggested):**

1) Implement FILETIME→`std::chrono::file_clock::time_point` conversion in `win32` scanner.
2) Add an optional `--scanner=win32` default on Windows builds.
3) Record first measurements using `fo_bench_scanner` on a stable dataset.
4) `--list-scanners` added to CLI; expose similar lists for hashers and metadata providers later.

**Where to Look / Edit:**

- Scanners: `core/src/scanner_*.cpp`
- Interfaces: `core/include/fo/core/interfaces.hpp`
- Types: `core/include/fo/core/types.hpp`
- Registry: `core/include/fo/core/registry.hpp`
- CLI: `cli/fo_cli.cpp`

**Decision Snapshot (ADR-0001):**

- Title: Default Scanner on Windows
- Context: `win32` vs `dirent` vs `std::filesystem` performance and metadata coverage
- Decision: Prefer `win32` as default on Windows; `std` remains default for cross-platform builds.
- Status: Proposed (awaiting benchmark confirmation)
- Consequences: Windows builds will include Win32-specific code paths; cross-platform behavior unchanged.
