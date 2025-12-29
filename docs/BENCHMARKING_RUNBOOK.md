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

## Running the Benchmark

We use Google Benchmark for microbenchmarks.

```powershell
./build/benchmarks/fo_benchmarks.exe
```

Outputs:
- **ScanStd**: Portable `std::filesystem` scanner performance.
- **ScanWin32**: Windows-specific `FindFirstFileExW` scanner performance.
- **BM_Hasher_Fast64**: Non-cryptographic fast hash throughput.
- **BM_Hasher_Blake3**: Cryptographic hash throughput.

## Sample Results (Dec 29, 2025)

Platform: 32x 3394 MHz CPU
Caches: L1 32K, L2 512K, L3 8M

| Benchmark | Time (ns) | CPU (ns) | Iterations | Throughput |
|-----------|-----------|----------|------------|------------|
| ScannerFixture/ScanStd | 3,147,196 | 3,069,196 | 224 | - |
| ScannerFixture/ScanWin32 | 3,492,099 | 3,459,821 | 280 | - |
| BM_Hasher_Fast64 | 686,622 | 680,106 | 896 | 1.43 GiB/s |
| BM_Hasher_Blake3 | 2,469,813 | 2,511,161 | 280 | 398.2 MiB/s |

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
