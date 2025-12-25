# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.3.0] - 2025-12-25

### Added
- **Database Layer**: Implemented SQLite integration with `DatabaseManager`, `FileRepository`, and `DuplicateRepository`.
- **Persistence**: Scanned files and duplicate groups are now persisted to `fo.db` (configurable via `--db`).
- **Schema**: Initial V1 schema with `files`, `file_hashes`, `duplicate_groups`, and `duplicate_members` tables.
- **CLI**: Added `--db=<path>` option.

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
