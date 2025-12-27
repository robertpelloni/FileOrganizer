# FileOrganizer Roadmap (Consolidated)

This roadmap synthesizes the full analysis, library evaluation, and your requirements into a phased implementation plan. All tasks support the goals of **extreme robustness, plugin architecture, cross-platform CLI-first design, and empirical benchmarking**.

---

## Current Status (Baseline)

### Completed
- ✅ Monolithic Qt app (`OpenFileOrganizer.cpp`) with scanning, hashing, metadata, and duplicate detection.
- ✅ Vendored libraries: TinyEXIF, hash-library, xxHash, SQLite3, and many others in `libs/`.
- ✅ Multiple directory scanning strategies (std::filesystem, dirent, Win32, manual).
- ✅ **Win32 Scanner Optimization**: Direct FILETIME to `std::chrono` conversion for high performance.
- ✅ Fast prefilter hash (sampled chunks) and optional strong hashes (SHA-256, MD5, etc.).
- ✅ EXIF date parsing (TinyEXIF) and filename regex-based date extraction.
- ✅ Size + fast-hash duplicate grouping with byte-compare verification.
- ✅ **Database Layer**: SQLite schema V1, migrations, and full repository layer (`FileRepository`, `DuplicateRepository`, `IgnoreRepository`, `ScanSessionRepository`).
- ✅ **CLI**: `fo_cli` with `scan`, `duplicates`, `hash`, `metadata` commands and `--db` persistence.
- ✅ **Documentation**: Centralized LLM instructions, Submodule Dashboard, Versioning.

### Gaps
- ❌ Tight Qt coupling: business logic in `Worker` QObject (legacy).
- ❌ No plugin registry: providers hardcoded, not swappable at runtime (Registry pattern implemented but dynamic loading not yet).
- ❌ No benchmarking harness: multiple implementations exist but not measured.
- ❌ No CI/CD: manual builds; no cross-platform testing.
- ❌ OCR and perceptual hashing not implemented (stubs only).
- ❌ Large vendor sprawl: many unused libraries (radare2, OpenCV, FFmpeg, etc.).

---

## Phase 1: Decouple and Architect (Weeks 1–3)

**Goal**: Extract a portable, CLI-first engine with a plugin architecture.

### Tasks

1. **Create CMake build system** ✅ (completed)
   - [x] Top-level `CMakeLists.txt` with `core/` and `cli/` subdirectories.
   - [x] Vendor TinyEXIF, hash-library, xxHash directly in core build.
   - [x] Add vcpkg manifest (`vcpkg.json`) for optional dependencies (Exiv2, BLAKE3, Tesseract).
   - [ ] Test cross-platform builds (Windows MSVC, Linux GCC/Clang, macOS Clang).

2. **Define core types and interfaces** ✅ (completed)
   - [x] `fo::core::types.hpp`: `FileInfo`, `DateMetadata`, `ImageMetadata`, `Hashes`.
   - [x] `fo::core::interfaces.hpp`: `IFileScanner`, `IHasher`, `IMetadataProvider`, `IDuplicateFinder`.
   - [x] `fo::core::ocr_interface.hpp`: `IOCRProvider`, `OCRResult`, `OCRBoundingBox`.
   - [x] `fo::core::registry.hpp`: Generic registry for static provider registration.

3. **Implement initial providers** ✅ (completed)
   - [x] Scanners: `StdFsScanner` (std::filesystem), `Win32Scanner`, `DirentScanner`.
   - [x] Hashers: `Fast64Hasher` (sampled), `SHA256Hasher` (hash-library), `XXHasher` (xxHash64).
   - [x] Metadata: `TinyExifMetadataProvider` (read-only).
   - [ ] Add stubs for: `Exiv2MetadataProvider`, `Blake3Hasher`, `TesseractOCRProvider` (feature-guarded).
   - [x] Implement `IDuplicateFinder` variants: `SizeHashDuplicateFinder`.

4. **Build CLI executable** ✅ (completed)
   - [x] `cli/fo_cli.cpp`: Parse args (`--scanner`, `--hasher`, `--ext`, roots).
   - [x] Integrate with `fo::core::Engine`.
   - [x] Add commands: `scan`, `duplicates`, `hash`, `metadata`.
   - [x] Add commands: `ocr` (subcommands).
   - [ ] Add JSON output mode (`--format=json`) for GUI consumption.

5. **Extract Qt GUI as thin client** (deferred to Phase 3)
   - [ ] Keep `OpenFileOrganizer.cpp` but refactor to call CLI via `QProcess` or link `fo_core` as library.
   - [ ] UI becomes a frontend; engine handles all logic.

**Deliverable**: A working `fo_cli` that can scan directories, compute hashes, and list duplicates without Qt.

---

## Phase 2: Database and Persistence (Weeks 4–5)

**Goal**: Robust schema with migrations and data-access layer.

### Tasks

1. **Implement database schema** ✅ (completed)
   - [x] Design tables: `files`, `file_dates`, `duplicate_groups`, `ignore_list`, `scan_sessions`, etc.
   - [x] Write initial migration SQL (v1).
   - [ ] Add perceptual hashing and OCR tables (v2, v3).

2. **Build migration manager** ✅ (completed)
   - [x] C++ class: `MigrationManager::apply_migrations(sqlite3* db)` (implemented in `DatabaseManager`).
   - [x] Store migrations as numbered SQL strings or files.
   - [x] Track applied versions in `schema_version` table.
   - [x] Test rollback safety (transactions per migration).

3. **Create data-access layer** ✅ (completed)
   - [x] `FileRepository`: CRUD for `files` and `file_dates`.
   - [x] `DuplicateRepository`: Insert groups, query by size/hash.
   - [x] `IgnoreRepository`: Manage exclude patterns.
   - [x] `ScanSessionRepository`: Track scan history.
   - [x] Use prepared statements; avoid SQL injection.

4. **Integrate DB into Engine** ✅ (completed)
   - [x] `Engine::scan()` inserts/updates `files` table.
   - [x] `Engine::find_duplicates()` populates `duplicate_groups` and `duplicate_members`.
   - [x] Add `--db-path` CLI flag (default: `~/.file_organizer/db.sqlite`).

5. **Optional: Alternate Data Streams (Windows)**
   - [ ] Implement ADS read/write for fast hashes (`file.jpg:fo_hash`).
   - [ ] CLI flag: `--use-ads-cache` (Windows only).
   - [ ] Fallback to DB if ADS unavailable (network shares, FAT32).

**Deliverable**: Persistent storage; incremental scans; query duplicates without re-hashing.

---

## Phase 3: Provider Expansion (Weeks 6–8)

**Goal**: Enable all optional providers with feature guards and vcpkg integration.

### Tasks

1. **Add vcpkg dependencies**
   - [x] `vcpkg.json`: Add `exiv2`, `blake3`, `tesseract`, `opencv` (optional).
   - [x] CMake `find_package()` for each; set `FO_HAVE_*` macros.
   - [x] Conditional compilation: `#ifdef FO_HAVE_EXIV2`.

2. **Implement advanced metadata providers**
   - [x] `Exiv2MetadataProvider`: Full EXIF/IPTC/XMP read/write (Code implemented, requires vcpkg build).
   - [ ] Test on JPEG, TIFF, HEIF, RAW (CR2, NEF, ARW).
   - [ ] Compare extraction rates vs. TinyEXIF (benchmarking).

3. **Implement strong hashers**
   - [x] `Blake3Hasher`: Fast cryptographic hashing (Code implemented, requires vcpkg build).
   - [ ] Benchmark vs. SHA-256 on large files.
   - [ ] Add to registry; select via `--hasher=blake3`.

4. **Implement OCR providers**
   - [x] `TesseractOCRProvider`: Basic text extraction (Code implemented, requires vcpkg build).
   - [ ] Optional: `PaddleOCRProvider` (Python bridge or C++ inference).
   - [ ] Store results in `ocr_results` table; enable FTS5 search.
   - [x] CLI: `fo_cli ocr --lang=eng /path/to/images`.

5. **Implement perceptual hashing**
   - [x] `dHash` (difference hash): Simple gradient-based.
   - [ ] `pHash` (DCT-based): Robust to rotation/scale.
   - [x] Store in `perceptual_hashes` table (via `file_repository`).
   - [x] CLI: `fo_cli similar --threshold=5 /path/to/query.jpg`.

6. **Add external tool integrations**
   - [ ] `ExifToolProvider`: Shell out to `exiftool` binary.
   - [ ] Opt-in via `--metadata-provider=exiftool`.

**Deliverable**: Full-featured engine with all providers selectable via CLI flags.

---

## Phase 4: Benchmarking and Optimization (Weeks 9–11)

**Goal**: Empirical validation of provider choices; performance tuning.

### Tasks

1. **Prepare benchmark datasets** ✅ (planned)
   - [ ] Acquire or generate small/medium/large/duplicates/EXIF datasets.
   - [ ] Label ground truth (duplicates, OCR text, perceptual similarity).
   - [ ] Store in `benchmarks/datasets/` (or download on demand).

2. **Implement benchmark harness** ✅ (completed)
   - [x] Add `benchmarks/fo_benchmarks.cpp` using Google Benchmark.
   - [x] Cover: scanners, hashers.
   - [ ] Output CSV for plotting; store baselines in repo.

3. **Run and analyze benchmarks**
   - [ ] Measure throughput, memory, accuracy on all datasets.
   - [ ] Identify optimal defaults (e.g., XXH3 for prefilter, BLAKE3 for strong, TinyEXIF for JPEG).
   - [ ] Document trade-offs in `LIBRARY_EVALUATION.md`.

4. **Optimize hot paths**
   - [ ] Profile with `perf` (Linux), VTune (Windows), Instruments (macOS).
   - [ ] Parallelize I/O-bound operations (multi-threaded hashing).
   - [ ] Reduce memory allocations (object pools, arena allocators).

5. **CI integration**
   - [x] GitHub Actions: build matrix (Windows/Linux/macOS, Debug/Release).
   - [ ] Run benchmarks weekly; upload results as artifacts.
   - [ ] Regression check: fail if throughput drops >10% without justification.

**Deliverable**: Data-driven provider defaults; optimized performance; CI-backed quality gates.

---

## Phase 5: GUI Decoupling and Frontends (Weeks 12–14)

**Goal**: Support multiple GUI shells (Qt, Electron, GTK, SDL) via stable IPC.

### Tasks

1. **Define IPC protocol**
   - [ ] Choose mechanism: JSON-RPC over stdin/stdout, named pipes, WebSocket, or msgpack.
   - [ ] Commands: `scan`, `duplicates`, `hash`, `metadata`, `ocr`, `status`, `cancel`.
   - [ ] Events: `progress`, `file_scanned`, `duplicate_found`, `error`.

2. **Implement IPC server in CLI**
   - [ ] `fo_cli serve --ipc=stdio` (JSON-RPC on stdin/stdout).
   - [ ] Optional: `--ipc=ws://localhost:9000` (WebSocket for Electron).
   - [ ] Stream progress events during long operations.

3. **Refactor Qt GUI**
   - [ ] Replace `Worker` with IPC client (`QProcess` or WebSocket).
   - [ ] UI sends commands; receives events via slots.
   - [ ] Test roundtrip latency (should be <10ms for local IPC).

4. **Prototype Electron frontend**
   - [ ] Basic UI: directory picker, start scan, view duplicates.
   - [ ] Communicate with `fo_cli serve` via WebSocket or child process.
   - [ ] Package as standalone app (Electron Builder).

5. **Document GUI integration guide**
   - [ ] Protocol spec: JSON schemas for commands/events.
   - [ ] Example clients: Qt, Electron, Python (CLI wrapper).

**Deliverable**: Decoupled architecture; Qt and Electron GUIs working with same engine.

---

## Phase 6: Advanced Features (Weeks 15–18)

**Goal**: AI classification, smart organization, and power-user workflows.

### Tasks

1. **AI-based classification**
   - [x] Integrate image classification model (ONNX Runtime).
   - [x] Tag images: `animal`, `landscape`, `document`, `screenshot`, etc.
   - [x] Store tags in `file_tags` table; enable search.
   - [x] CLI: `fo_cli classify --model=mobilenet /path/to/images`.

2. **Smart organization rules**
   - [x] Define rules: "Move photos from 2023 to `/Photos/2023/`" (RuleEngine implemented).
   - [x] Template variables: `{year}`, `{month}`, `{camera_make}`, `{tag}`.
   - [x] Dry-run mode: preview moves without executing.
   - [x] CLI: `fo_cli organize --rule="/Photos/{year}/{month}" --dry-run`.
   - [x] Support YAML rule files (currently single rule via CLI).

3. **Batch operations**
   - [x] CLI: `fo_cli delete-duplicates --keep=oldest --auto`.
   - [x] CLI: `fo_cli rename --pattern="{date}_{name}.{ext}"`.
   - [ ] Undo support: track operations in `operation_log` table.

4. **Incremental scanning**
   - [ ] Track file mtimes in DB; skip unchanged files.
   - [ ] CLI: `fo_cli scan --incremental`.
   - [ ] Detect moved files (same hash, new path); prompt to update DB.

5. **Export and reporting**
   - [ ] CLI: `fo_cli export --format=json|csv|html /path/to/report`.
   - [ ] Generate HTML report with thumbnails, duplicate lists, statistics.

**Deliverable**: Production-ready tool with automation and power-user workflows.

---

## Phase 7: Polish and Release (Weeks 19–20)

**Goal**: Documentation, packaging, and public launch.

### Tasks

1. **Documentation**
   - [ ] User manual: installation, CLI commands, GUI usage.
   - [ ] Developer guide: building, adding providers, contributing.
   - [ ] API reference: Doxygen for core headers.

2. **Packaging**
   - [ ] Windows: MSI installer (WiX) or NSIS.
   - [ ] Linux: AppImage, Flatpak, or distro packages (.deb, .rpm).
   - [ ] macOS: .dmg with notarized binary.
   - [ ] Portable: ZIP with CLI + optional GUI.

3. **Testing**
   - [ ] Unit tests: Catch2/Google Test for core functions.
   - [ ] Integration tests: End-to-end CLI workflows.
   - [ ] Fuzz testing: Random inputs to scanner/parser (libFuzzer).

4. **Release**
   - [ ] Tag v1.0.0; publish on GitHub Releases.
   - [ ] Announce on Reddit (r/DataHoarder, r/selfhosted), Hacker News, etc.
   - [ ] Website: robertpelloni.com/file-organizer or dedicated site.

**Deliverable**: Public release with installers, docs, and community support.

---

## Summary Table

| Phase | Weeks | Goal | Key Deliverables |
|-------|-------|------|------------------|
| 1. Decouple | 1–3 | CLI-first engine | CMake build, plugin registry, `fo_cli` |
| 2. Database | 4–5 | Persistent storage | Migrations, data-access layer, incremental scans |
| 3. Providers | 6–8 | Full feature set | Exiv2, BLAKE3, Tesseract, perceptual hashing |
| 4. Benchmarks | 9–11 | Empirical validation | Datasets, harness, CI integration |
| 5. GUI | 12–14 | Multi-frontend | IPC protocol, Qt refactor, Electron prototype |
| 6. Advanced | 15–18 | Power features | AI classification, smart organization, batch ops |
| 7. Release | 19–20 | Public launch | Installers, docs, v1.0.0 release |

---

## Open Questions and Decisions

1. **Radare2**: Remove from vendor tree? (Yes, unless binary analysis is planned.)
2. **Alternate Data Streams**: Implement as optional cache or defer? (Optional, Phase 2.)
3. **AI models**: On-device (ONNX Runtime) or cloud API (OpenAI CLIP)? (On-device for privacy.)
4. **GUI default**: Ship Qt, Electron, or both? (Both, but promote Electron for broader appeal.)
5. **License**: Stick with GPL or switch to MIT/Apache? (GPL is fine; Exiv2 requires it anyway.)

---

## Next Immediate Steps (This Week)

1. ✅ Finalize CMake build with vendored libs.
2. ✅ Complete initial providers (scanners, hashers, metadata).
3. ✅ Test `fo_cli` on sample directory.
4. ✅ Implement database schema and migrations.
5. ✅ Add CLI subcommands (`scan`, `duplicates`, `hash`, `ocr`, `similar`, `classify`).
6. ✅ Write benchmark harness skeleton.
7. ⏳ Run benchmarks and optimize.
8. ⏳ Implement smart organization rules.

---

## Long-Term Vision & Feature Backlog

For a detailed list of potential features and ideas for development beyond the v1.0 roadmap, see the following documents:

- **[Competitive Analysis](COMPETITIVE_ANALYSIS.md)**: A review of features from other leading file management and deduplication tools.
- **[Feature Wishlist](FEATURE_WISHLIST.md)**: A backlog of potential new features, including advanced duplicate detection, filesystem "lint" finding, and workflow enhancements.

### High-Level Vision (Beyond v1.0)
- **Cloud Sync:** Optional integration with S3, Google Drive for backup verification.
- **Web UI:** React/Vue frontend hosted locally; access via browser.
- **Mobile Apps:** iOS/Android clients (read-only; view duplicates, browse metadata).
- **Plugin Marketplace:** A system for community-contributed providers (new hash algorithms, OCR engines, AI models).
- **Enterprise Features:** LDAP authentication, audit logs, and policy enforcement for corporate NAS environments.

---

## Alignment with Your Goals

✅ **All features important**: Preserved and expanded (EXIF, hashing, OCR, AI, duplicates).  
✅ **Redundant operations for benchmarking**: Multiple scanners/hashers retained; harness planned.  
✅ **Cross-platform from start**: CMake + vcpkg; tested on Windows/Linux/macOS.  
✅ **Decoupled from Qt**: CLI-first engine; GUI as thin client via IPC.  
✅ **Plugin architecture**: Registry pattern; providers swappable at runtime.  
✅ **Evaluate all libraries**: Systematic matrix with speed/accuracy/license comparison.  
✅ **Extremely robust**: Benchmarking, CI, migration-safe DB, feature guards.  
✅ **Long-term commitment**: 20-week roadmap; extensible for future features.  
✅ **GPL acceptable**: License unblocked for Exiv2, Tesseract, etc.  

**Radare2**: Binary analysis framework; not needed for file organization—safe to remove or exclude from builds to reduce bloat.
