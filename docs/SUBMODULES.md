# Submodule Dashboard

This document provides an overview of the 130+ git submodules in `libs/`.

> **Note:** A detailed, programmatically generated dashboard with license, version, and usage information will be generated here by the `generate_dashboard.py` script.

## Summary

- **Total Submodules**: 130+
- **Location**: `libs/` directory
- **Purpose**: Vendored dependencies for offline builds and reproducibility.

## Key Dependencies

| Library | Purpose | License |
|---------|---------|---------|
| `TinyEXIF` | EXIF parsing | MIT |
| `hash-library` | SHA256, MD5 | zlib |
| `xxHash` | Fast hashing | BSD-2-Clause |
| `sqlite3` | Database | Public Domain |
| `CLI11` | Command-line parsing | BSD-3-Clause |
| `googletest` | Unit testing | BSD-3-Clause |
| `benchmark` | Performance benchmarking | Apache-2.0 |

## Management

To update all submodules:
```powershell
git submodule update --init --recursive
```

To update to the latest upstream version:
```powershell
git submodule foreach "git fetch origin && git checkout origin/HEAD"
```
