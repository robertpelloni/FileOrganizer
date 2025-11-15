# FileOrganizer CLI Engine (prototype)

This is a cross-platform, console-first engine scaffold extracted from the monolithic Qt app. It scans directories, computes a fast prefilter hash, and groups potential duplicates by size + hash. It is designed for a plugin architecture so multiple providers (scanner, hasher, metadata, OCR, DB) can be swapped and benchmarked.

## Build (CMake)

Requirements: CMake 3.16+, a C++20 compiler.

```bash
# Configure and build (out-of-source)
cmake -S . -B build
cmake --build build --config Release

# Run
./build/cli/fo_cli --help
```

On Windows (PowerShell):

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
.\build\cli\Release\fo_cli.exe --help
```

## Usage

```
fo_cli [--scanner=std|win32|dirent] [--hasher=fast64] [--ext=.jpg,.png] [--follow-symlinks] [--list-scanners] DIR...
```

- `--scanner=std`: C++20 std::filesystem recursive scanner (portable default)
- `--scanner=win32` (Windows only): Native Win32 API (fastest, richest metadata)
- `--scanner=dirent`: POSIX-style opendir/readdir + stat (competitive speed, basic metadata)
- `--hasher=fast64`: non-cryptographic sampled 64-bit prefilter hash
- `--ext=...`: comma-separated list of file extensions (with or without dots)
- `--follow-symlinks`: follow directory symlinks
- `--list-scanners`: print available scanner providers and exit

Outputs timing and lists duplicate groups found by size + fast64.

## Notes

- This prototype intentionally avoids heavy external dependencies. Strong hashes (SHA-256, BLAKE3), EXIF readers (TinyEXIF/Exiv2), and OCR providers will be integrated as separate providers next.
- The plugin registry supports multiple implementations per feature; selection via CLI flags will be extended.

See `docs/SCANNER_EVALUATION.md` for analysis of scanner trade-offs on Windows and Linux.