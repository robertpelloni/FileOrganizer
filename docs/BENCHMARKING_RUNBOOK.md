# Benchmarking Runbook

This runbook explains how to measure scanner throughput and produce repeatable results.

## Prerequisites
- CMake 3.16+
- A C++20 compiler (MSVC/Clang/GCC) available in PATH or via Visual Studio generator

## Build the Scanner Benchmark

Windows (Visual Studio):
```powershell
"C:\Program Files\CMake\bin\cmake.exe" -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
```

Windows (Ninja + compiler in PATH):
```powershell
"C:\Program Files\CMake\bin\cmake.exe" -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Linux/macOS:
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
```

## Running the Benchmark

```bash
# Basic run (portable scanner)
./build/benchmarks/fo_bench_scanner --scanner=std --iters=5 /path/to/dir

# Windows native scanner
./build/benchmarks/fo_bench_scanner --scanner=win32 --iters=5 C:\\path\\to\\dir

# dirent-based scanner (POSIX or Windows shim)
./build/benchmarks/fo_bench_scanner --scanner=dirent --iters=5 /path/to/dir

# Filter extensions
./build/benchmarks/fo_bench_scanner --scanner=std --ext=.jpg,.png --iters=5 /photos
```

Outputs per run: file count, elapsed seconds, throughput (files/s). Summary prints median and p90 across runs.

## Measurement Protocol
- Warm and cold cache: run two sets to understand filesystem cache effects.
- Repeat 5× and record median + p90.
- Use consistent datasets; avoid antivirus rescans during measurement when possible.
- Report environment (CPU, RAM, OS build), command, date/time.

## Recording Results
- Append results to `docs/AI_HANDOFF.md` under a new dated section with:
  - Provider, dataset, mode (names | names+stat)
  - Files/dirs counted
  - Median throughput and p90
  - Notes about errors or edge cases

For deeper evaluation methodology, see `docs/SCANNER_EVALUATION.md`.
