# Feature Wishlist & Backlog

This document synthesizes the existing project roadmap and findings from the competitive analysis to create a definitive list of potential features. Its purpose is to serve as a high-level backlog for future development beyond the v1.0 release.

---

## Core Roadmap (v1.0)

The current roadmap is ambitious and covers a robust set of features planned for the initial major release. A summary of these core features is provided below for context. See `docs/ROADMAP.md` for the full, phased implementation plan.

### Key Features (Phases 1-7)
- **CLI-First Engine:** A portable, high-performance C++20 core engine.
- **Plugin Architecture:** Swappable providers for all major functions:
  - **Scanners:** `std::filesystem`, native Win32, `dirent`.
  - **Hashers:** `xxHash` (pre-filter), `BLAKE3`/`SHA-256` (strong verification).
  - **Metadata:** `TinyEXIF`, `Exiv2` (full EXIF/IPTC/XMP), filename date parsing.
  - **OCR:** `Tesseract`, `PaddleOCR`.
- **Database & Persistence:** Robust SQLite backend with schema migrations for incremental scanning and persistent metadata.
- **Duplicate Detection:**
  - Exact duplicates (Size + Hash + optional Byte-Compare).
  - Perceptual hashing for similar images (`dHash`, `pHash`).
- **Advanced Capabilities:**
  - AI-based image classification (tagging).
  - Smart organization rules (scriptable move/rename operations).
  - Batch operations with dry-run and undo support.
- **Cross-Platform Support:** Windows, Linux, and macOS with dedicated GUIs (Qt, Electron) over a stable IPC layer.
- **Empirical Benchmarking:** A full suite to measure and validate the performance of all providers.

---

## Future Features (Post-v1.0 Backlog)

This section contains features inspired by other leading file management tools and potential extensions to the core engine. They are candidates for development after the v1.0 roadmap is complete.

### 1. Advanced Duplicate & Similarity Detection

These features extend the core duplicate-finding capabilities to handle more complex or media-specific scenarios.

- **Fuzzy Filename Matching:**
  - *Description:* An algorithm to find files with similar, but not identical, filenames (e.g., `photo_1.jpg` vs. `photo 1.jpg` vs. `photo_1_copy.jpg`).
  - *Inspired by:* dupeGuru.

- **Music Duplicate Detection (by Tags):**
  - *Description:* A specialized mode to find duplicate music files by comparing audio tags (Artist, Album, Title, Track Number, Year) rather than just file content. This can identify the same song in different formats or bitrates.
  - *Inspired by:* dupeGuru, Czkawka.

- **Similar Video Detection:**
  - *Description:* A perceptual hashing equivalent for video files. This would involve extracting keyframes and comparing their visual similarity to find duplicate or near-duplicate videos, even with different encodings, resolutions, or minor edits.
  - *Inspired by:* Czkawka.

### 2. Filesystem Health & "Lint" Finding

This category expands the tool's scope from organization to general filesystem hygiene, helping users clean up various types of clutter and broken files.

- **Broken Symlink Detection:**
  - *Description:* Identify symbolic links that point to non-existent targets.
  - *Inspired by:* rmlint, Czkawka.

- **Empty File & Directory Finder:**
  - *Description:* Recursively find and list empty files and directories, which often contribute to filesystem clutter.
  - *Inspired by:* rmlint, Czkawka.

- **Temporary File Finder:**
  - *Description:* Search for files based on common temporary file patterns (e.g., `*.tmp`, `*.bak`, `~*`) to help users reclaim space.
  - *Inspired by:* Czkawka.

- **Broken File Detection:**
  - *Description:* A module to identify corrupted or truncated files. This could involve checking for invalid headers in media files (JPEG, PNG, MP4) or unexpected EOF markers.
  - *Inspired by:* Czkawka.

- **Non-stripped Binary Detection:**
  - *Description:* Find executable files and libraries that contain debug symbols, which are often unnecessary for production use and consume extra space.
  - *Inspired by:* rmlint.

### 3. Filesystem Integration & Optimization

These features involve deeper integration with modern filesystem capabilities for enhanced performance and space savings.

- **Block-Level Deduplication (Reflink/CoW):**
  - *Description:* Instead of deleting duplicate files, use filesystem-level "reflink" or "clone" operations to make multiple filenames point to the same data blocks on disk. This saves space without deleting any file entries.
  - *Dependencies:* Requires a supported filesystem (BTRFS, XFS, APFS).
  - *Inspired by:* jdupes, rmlint.

### 4. Usability & Workflow Enhancements

- **Scan Result Caching & Replay:**
  - *Description:* The ability to save a complete scan result (including hashes and file lists) to a file (e.g., JSON or a DB snapshot) and "replay" it later. This allows for re-filtering, re-sorting, or generating different reports without re-scanning the filesystem.
  - *Inspired by:* rmlint.
