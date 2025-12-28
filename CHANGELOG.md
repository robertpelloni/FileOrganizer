# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.6.0] - 2025-12-28

### Added
- **Export Feature**: New `export` command for exporting scan results.
    - Supports JSON, CSV, and HTML formats (`--format=json|csv|html`).
    - Includes file statistics, duplicates summary, and full file listing.
    - Output to stdout or file (`--output=<path>`).
- **Export Module**: Added `core/include/fo/core/export.hpp` and `core/src/export.cpp`.
- **CLI**: Added `--output=<path>` option for export file destination.

### Changed
- Updated `print_usage()` to document the new export command.

## [0.5.1] - 2025-12-27

### Added
- **Submodule Dashboard**: Automated generation of `docs/SUBMODULES.md` listing all submodules, versions, and build info.
- **Documentation**: Added project structure explanation to the dashboard.

### Changed
- **Submodules**: Updated all submodules to their latest upstream versions.
- **Build**: Verified build stability with updated dependencies.

## [0.5.0] - 2025-12-27

### Added
- **Smart Organization**: Implemented `RuleEngine` for organizing files based on metadata templates (e.g., `/Photos/{year}/{month}/`).
- **YAML Rules**: Added support for loading organization rules from YAML files (`--rules=rules.yaml`).
- **Batch Operations**:
    - `delete-duplicates`: Bulk deletion of duplicates with strategies (`oldest`, `newest`, `shortest`, `longest`).
    - `rename`: Bulk renaming using patterns (e.g., `{year}-{month}-{day}_{name}.{ext}`).
- **CLI Commands**: Added `organize`, `delete-duplicates`, and `rename` commands.
- **Dependencies**: Added `yaml-cpp` to `vcpkg.json`.

## [0.4.0] - 2025-12-25

### Added
- **AI Classification**: Implemented `OnnxRuntimeClassifier` for image tagging using ONNX models.
- **Perceptual Hashing**: Implemented `OpencvPerceptualHasher` (dHash) for finding visually similar images.
- **Benchmarking**: Added Google Benchmark harness (`benchmarks/fo_benchmarks.cpp`) for scanners and hashers.
- **CLI Commands**: Added `classify` (AI tagging) and `similar` (perceptual hash search) commands.
- **CLI Commands**: Added `download-models` command (stub) to help users acquire AI models.
- **Database**: Added `tags` and `file_tags` tables (Migration 2) for storing AI classification results.
- **Persistence**: `classify` command now saves tags to the database.
- **Dependencies**: Added `opencv`, `onnxruntime`, and `benchmark` to `vcpkg.json`.

## [0.3.0] - 2025-12-25

### Added
- **CI/CD**: Added GitHub Actions workflow for Windows and Linux builds.
- **Dependencies**: Added `vcpkg.json` for managing Tesseract, BLAKE3, Exiv2, and SQLite3.
- **Providers**: Implemented `Blake3Hasher` and `Exiv2MetadataProvider` (feature-guarded).
- **Database Layer**: Implemented SQLite integration with `DatabaseManager`, `FileRepository`, and `DuplicateRepository`.
- **Persistence**: Scanned files and duplicate groups are now persisted to `fo.db` (configurable via `--db`).
- **Schema**: Initial V1 schema with `files`, `file_hashes`, `duplicate_groups`, and `duplicate_members` tables.
- **CLI**: Added `--db=<path>` option.
- **CLI Commands**: Implemented `hash`, `metadata`, and `ocr` commands.
- **OCR**: Implemented `TesseractOCRProvider` (requires `FO_HAVE_TESSERACT` build flag).
- **Versioning**: Added `--version` flag to CLI; build system now reads version from `VERSION.md`.
- **Repositories**: Added `IgnoreRepository` and `ScanSessionRepository`.
- **Submodules**: Fixed missing `vcpkg` submodule mapping.

## [0.2.0] - 2025-12-25

### Added
- **Submodule Dashboard**: Created `docs/SUBMODULES.md` listing all submodules and their versions.
- **Versioning**: Introduced `VERSION.md` as the single source of truth for the project version.
- **LLM Instructions**: Centralized AI agent instructions in `docs/LLM_INSTRUCTIONS.md`.

### Changed
- Updated `AGENTS.md` to reference the new universal instructions.
- Updated submodules to their latest local state.

### Fixed
- Addressed missing submodule mapping for `vcpkg`.

## [0.1.0] - 2025-11-14

### Added
- Initial release of FileOrganizer CLI.
- Scanners: `std`, `win32`, `dirent`.
- Basic benchmarking tools.
