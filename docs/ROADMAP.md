# FileOrganizer Roadmap (Consolidated)

This roadmap synthesizes the full analysis, library evaluation, and your requirements into a phased implementation plan. All tasks support the goals of **extreme robustness, plugin architecture, cross-platform CLI-first design, and empirical benchmarking**.

---

## Current Status (Baseline)

### Completed
- ✅ Monolithic Qt app (`OpenFileOrganizer.cpp`) with scanning, hashing, metadata, and duplicate detection.
- ✅ Vendored libraries: TinyEXIF, hash-library, xxHash, SQLite3, and many others in `libs/`.
- ✅ Multiple directory scanning strategies (std::filesystem, dirent, Win32, manual).
- ✅ Fast prefilter hash (sampled chunks) and optional strong hashes (SHA-256, MD5, etc.).
- ✅ EXIF date parsing (TinyEXIF) and filename regex-based date extraction.
- ✅ Size + fast-hash duplicate grouping with byte-compare verification.
- ✅ Preliminary SQLite schema (ad hoc inserts; no migrations).

### Gaps
- ❌ Tight Qt coupling: business logic in `Worker` QObject.
- ❌ No CLI-first engine: cannot use without Qt GUI.
- ❌ No plugin registry: providers hardcoded, not swappable at runtime.
- ❌ No benchmarking harness: multiple implementations exist but not measured.
- ❌ No database migrations: schema changes break existing DBs.
- ❌ No CI/CD: manual builds; no cross-platform testing.
- ❌ OCR and perceptual hashing not implemented.
- ❌ Large vendor sprawl: many unused libraries (radare2, OpenCV, FFmpeg, etc.).

---

## Phase 1: Decouple and Architect (Weeks 1–3)

**Goal**: Extract a portable, CLI-first engine with a plugin architecture.

### Tasks

1. **Create CMake build system**
   - [x] Top-level `CMakeLists.txt` with `core/` and `cli/` subdirectories.
   - [x] Vendor TinyEXIF, hash-library, xxHash directly in core build.
   - [x] Add vcpkg manifest (`vcpkg.json`) for optional dependencies (Exiv2, BLAKE3, Tesseract).
   - [ ] Test cross-platform builds (Windows MSVC, Linux GCC/Clang, macOS Clang).
   - **Status:** ✅ **Completed**

2. **Define core types and interfaces**
   - [x] `fo::core::types.hpp`: `FileInfo`, `DateMetadata`, `ImageMetadata`, `Hashes`.
   - [x] `fo::core::interfaces.hpp`: `IFileScanner`, `IHasher`, `IMetadataProvider`, `IDuplicateFinder`.
   - [x] `fo::core::ocr_interface.hpp`: `IOCRProvider`, `OCRResult`, `OCRBoundingBox`.
   - [x] `fo::core::registry.hpp`: Generic registry for static provider registration.
   - **Status:** ✅ **Completed**

3. **Implement initial providers**
   - [x] Scanners: `StdFsScanner` (std::filesystem).
   - [x] Hashers: `Fast64Hasher` (sampled), `SHA256Hasher` (hash-library), `XXHasher` (xxHash64).
   - [x] Metadata: `TinyExifMetadataProvider` (read-only).
   - [x] Add stubs for: `Exiv2MetadataProvider`, `Blake3Hasher`, `TesseractOCRProvider` (feature-guarded).
   - [x] Implement `IDuplicateFinder` variants: `SizeHashDuplicateFinder`, `SizeHashByteDuplicateFinder`.
   - **Status:** ✅ **Completed**

4. **Build CLI executable**
   - [x] `cli/fo_cli.cpp`: Parse args (`--scanner`, `--hasher`, `--ext`, roots).
   - [x] Integrate with `fo::core::Engine`.
   - [ ] Add commands: `scan`, `duplicates`, `hash`, `metadata`, `ocr` (subcommands).
   - [ ] Add JSON output mode (`--format=json`) for GUI consumption.
   - **Status:** ⚠️ **In Progress**

5. **Extract Qt GUI as thin client** (deferred to Phase 3)
   - [ ] Keep `OpenFileOrganizer.cpp` but refactor to call CLI via `QProcess` or link `fo_core` as library.
   - [ ] UI becomes a frontend; engine handles all logic.
   - **Status:** 🕒 **Pending**

**Deliverable**: A working `fo_cli` that can scan directories, compute hashes, and list duplicates without Qt.

---

## Phase 2: Database and Persistence (Weeks 4–5)

**Goal**: Robust schema with migrations and data-access layer.

### Tasks

1. **Implement database schema**
   - [x] Design tables: `files`, `file_dates`, `duplicate_groups`, `ignore_list`, `scan_sessions`, etc.
   - [ ] Write initial migration SQL (v1).
   - [ ] Add perceptual hashing and OCR tables (v2, v3).
   - **Status:** ⚠️ **In Progress**

2. **Build migration manager**
   - [ ] C++ class: `MigrationManager::apply_migrations(sqlite3* db)`.
   - [ ] Store migrations as numbered SQL strings or files.
   - [ ] Track applied versions in `schema_version` table.
   - [ ] Test rollback safety (transactions per migration).
   - **Status:** 🕒 **Pending**

3. **Create data-access layer**
   - [ ] `FileRepository`: CRUD for `files` and `file_dates`.
   - [ ] `DuplicateRepository`: Insert groups, query by size/hash.
   - [ ] `IgnoreRepository`: Manage exclude patterns.
   - [ ] `ScanSessionRepository`: Track scan history.
   - [ ] Use prepared statements; avoid SQL injection.
   - **Status:** 🕒 **Pending**

4. **Integrate DB into Engine**
   - [ ] `Engine::scan()` inserts/updates `files` table.
   - [ ] `Engine::find_duplicates()` populates `duplicate_groups` and `duplicate_members`.
   - [ ] Add `--db-path` CLI flag (default: `~/.file_organizer/db.sqlite`).
   - **Status:** 🕒 **Pending**

5. **Optional: Alternate Data Streams (Windows)**
   - [ ] Implement ADS read/write for fast hashes (`file.jpg:fo_hash`).
   - [ ] CLI flag: `--use-ads-cache` (Windows only).
   - [ ] Fallback to DB if ADS unavailable (network shares, FAT32).
   - **Status:** 🕒 **Pending**

**Deliverable**: Persistent storage; incremental scans; query duplicates without re-hashing.

---

## Phase 3: Provider Expansion (Weeks 6–8)

**Goal**: Enable all optional providers with feature guards and vcpkg integration.

### Tasks

1. **Add vcpkg dependencies**
   - [x] `vcpkg.json`: Add `exiv2`, `blake3`, `tesseract`, `opencv` (optional).
   - [x] CMake `find_package()` for each; set `FO_HAVE_*` macros.
   - [x] Conditional compilation: `#ifdef FO_HAVE_EXIV2`.
   - **Status:** ✅ **Completed**

2. **Implement advanced metadata providers**
   - [ ] `Exiv2MetadataProvider`: Full EXIF/IPTC/XMP read/write.
   - [ ] Test on JPEG, TIFF, HEIF, RAW (CR2, NEF, ARW).
   - [ ] Compare extraction rates vs. TinyEXIF (benchmarking).
   - **Status:** 🕒 **Pending**

3. **Implement strong hashers**
   - [x] `Blake3Hasher`: Fast cryptographic hashing.
   - [ ] Benchmark vs. SHA-256 on large files.
   - [x] Add to registry; select via `--hasher=blake3`.
   - **Status:** ✅ **Completed**

4. **Implement OCR providers**
   - [x] `TesseractOCRProvider`: Basic text extraction.
   - [ ] Optional: `PaddleOCRProvider` (Python bridge or C++ inference).
   - [ ] Store results in `ocr_results` table; enable FTS5 search.
   - [ ] CLI: `fo_cli ocr --lang=eng /path/to/images`.
   - **Status:** ⚠️ **In Progress**

5. **Implement perceptual hashing**
   - [x] `dHash` (difference hash): Simple gradient-based.
   - [ ] `pHash` (DCT-based): Robust to rotation/scale.
   - [ ] Store in `perceptual_hashes` table.
   - [ ] CLI: `fo_cli similar --threshold=5 /path/to/query.jpg`.
   - **Status:** ⚠️ **In Progress**

6. **Add external tool integrations**
   - [ ] `ExifToolProvider`: Shell out to `exiftool` binary.
   - [ ] Opt-in via `--metadata-provider=exiftool`.
   - **Status:** 🕒 **Pending**

**Deliverable**: Full-featured engine with all providers selectable via CLI flags.

---

## Phase 4: Benchmarking and Optimization (Weeks 9–11)

**Goal**: Empirical validation of provider choices; performance tuning.

### Tasks

1. **Prepare benchmark datasets**
   - [ ] Acquire or generate small/medium/large/duplicates/EXIF datasets.
   - [ ] Label ground truth (duplicates, OCR text, perceptual similarity).
   - [ ] Store in `benchmarks/datasets/` (or download on demand).
   - **Status:** 🕒 **Pending**

2. **Implement benchmark harness**
   - [x] Add `benchmarks/fo_benchmarks.cpp` using Google Benchmark.
   - [ ] Cover: scanners, hashers, metadata, duplicate detection, OCR, perceptual hashing.
   - [ ] Output CSV for plotting; store baselines in repo.
   - **Status:** ⚠️ **In Progress**

3. **Run and analyze benchmarks**
   - [ ] Measure throughput, memory, accuracy on all datasets.
   - [ ] Identify optimal defaults (e.g., XXH3 for prefilter, BLAKE3 for strong, TinyEXIF for JPEG).
   - [ ] Document trade-offs in `LIBRARY_EVALUATION.md`.
   - **Status:** 🕒 **Pending**

4. **Optimize hot paths**
   - [ ] Profile with `perf` (Linux), VTune (Windows), Instruments (macOS).
   - [ ] Parallelize I/O-bound operations (multi-threaded hashing).
   - [ ] Reduce memory allocations (object pools, arena allocators).
   - **Status:** 🕒 **Pending**

5. **CI integration**
   - [ ] GitHub Actions: build matrix (Windows/Linux/macOS, Debug/Release).
   - [ ] Run benchmarks weekly; upload results as artifacts.
   - [ ] Regression check: fail if throughput drops >10% without justification.
   - **Status:** 🕒 **Pending**

**Deliverable**: Data-driven provider defaults; optimized performance; CI-backed quality gates.

---

## Phase 5: GUI Decoupling and Frontends (Weeks 12–14)

**Goal**: Support multiple GUI shells (Qt, Electron, GTK, SDL) via stable IPC.

### Tasks

1. **Define IPC protocol**
   - [ ] Choose mechanism: JSON-RPC over stdin/stdout, named pipes, WebSocket, or msgpack.
   - [ ] Commands: `scan`, `duplicates`, `hash`, `metadata`, `ocr`, `status`, `cancel`.
   - [ ] Events: `progress`, `file_scanned`, `duplicate_found`, `error`.
   - **Status:** 🕒 **Pending**

2. **Implement IPC server in CLI**
   - [ ] `fo_cli serve --ipc=stdio` (JSON-RPC on stdin/stdout).
   - [ ] Optional: `--ipc=ws://localhost:9000` (WebSocket for Electron).
   - [ ] Stream progress events during long operations.
   - **Status:** 🕒 **Pending**

3. **Refactor Qt GUI**
   - [ ] Replace `Worker` with IPC client (`QProcess` or WebSocket).
   - [ ] UI sends commands; receives events via slots.
   - [ ] Test roundtrip latency (should be <10ms for local IPC).
   - **Status:** 🕒 **Pending**

4. **Prototype Electron frontend**
   - [ ] Basic UI: directory picker, start scan, view duplicates.
   - [ ] Communicate with `fo_cli serve` via WebSocket or child process.
   - [ ] Package as standalone app (Electron Builder).
   - **Status:** 🕒 **Pending**

5. **Document GUI integration guide**
   - [ ] Protocol spec: JSON schemas for commands/events.
   - [ ] Example clients: Qt, Electron, Python (CLI wrapper).
   - **Status:** 🕒 **Pending**

**Deliverable**: Decoupled architecture; Qt and Electron GUIs working with same engine.

---

## Phase 6: Advanced Features (Weeks 15–18)

**Goal**: AI classification, smart organization, and power-user workflows.

### Tasks

1. **AI-based classification**
   - [ ] Integrate image classification model (e.g., MobileNetV2, CLIP).
   - [ ] Tag images: `animal`, `landscape`, `document`, `screenshot`, etc.
   - [ ] Store tags in `file_tags` table; enable search.
   - [ ] CLI: `fo_cli classify --model=mobilenet /path/to/images`.
   - **Status:** 🕒 **Pending**

2. **Smart organization rules**
   - [ ] Define rules: "Move photos from 2023 to `/Photos/2023/`".
   - [ ] Template variables: `{year}`, `{month}`, `{camera_make}`, `{tag}`.
   - [ ] Dry-run mode: preview moves without executing.
   - [ ] CLI: `fo_cli organize --rules=rules.yaml --dry-run`.
   - **Status:** 🕒 **Pending**

3. **Batch operations**
   - [ ] CLI: `fo_cli delete-duplicates --keep=oldest --auto`.
   - [ ] CLI: `fo_cli rename --pattern="{date}_{name}.{ext}"`.
   - [ ] Undo support: track operations in `operation_log` table.
   - **Status:** 🕒 **Pending**

4. **Incremental scanning**
   - [ ] Track file mtimes in DB; skip unchanged files.
   - [ ] CLI: `fo_cli scan --incremental`.
   - [ ] Detect moved files (same hash, new path); prompt to update DB.
   - **Status:** 🕒 **Pending**

5. **Export and reporting**
   - [ ] CLI: `fo_cli export --format=json|csv|html /path/to/report`.
   - [ ] Generate HTML report with thumbnails, duplicate lists, statistics.
   - **Status:** 🕒 **Pending**

**Deliverable**: Production-ready tool with automation and power-user workflows.

---

## Phase 7: Polish and Release (Weeks 19–20)

**Goal**: Documentation, packaging, and public launch.

### Tasks

1. **Documentation**
   - [ ] User manual: installation, CLI commands, GUI usage.
   - [ ] Developer guide: building, adding providers, contributing.
   - [ ] API reference: Doxygen for core headers.
   - **Status:** 🕒 **Pending**

2. **Packaging**
   - [ ] Windows: MSI installer (WiX) or NSIS.
   - [ ] Linux: AppImage, Flatpak, or distro packages (.deb, .rpm).
   - [ ] macOS: .dmg with notarized binary.
   - [ ] Portable: ZIP with CLI + optional GUI.
   - **Status:** 🕒 **Pending**

3. **Testing**
   - [ ] Unit tests: Catch2/Google Test for core functions.
   - [ ] Integration tests: End-to-end CLI workflows.
   - [ ] Fuzz testing: Random inputs to scanner/parser (libFuzzer).
   - **Status:** 🕒 **Pending**

4. **Release**
   - [ ] Tag v1.0.0; publish on GitHub Releases.
   - [ ] Announce on Reddit (r/DataHoarder, r/selfhosted), Hacker News, etc.
   - [ ] Website: robertpelloni.com/file-organizer or dedicated site.
   - **Status:** 🕒 **Pending**

**Deliverable**: Public release with installers, docs, and community support.

---

## Deliverables and Milestones

| Deliverable | Milestone | Status |
| --- | --- | --- |
| A working `fo_cli` that can scan directories, compute hashes, and list duplicates without Qt. | Phase 1 | ⚠️ **In Progress** |
| Persistent storage; incremental scans; query duplicates without re-hashing. | Phase 2 | 🕒 **Pending** |
| Full-featured engine with all providers selectable via CLI flags. | Phase 3 | ⚠️ **In Progress** |
| Data-driven provider defaults; optimized performance; CI-backed quality gates. | Phase 4 | 🕒 **Pending** |
| Decoupled architecture; Qt and Electron GUIs working with same engine. | Phase 5 | 🕒 **Pending** |
| Production-ready tool with automation and power-user workflows. | Phase 6 | 🕒 **Pending** |
| Public release with installers, docs, and community support. | Phase 7 | 🕒 **Pending** |
