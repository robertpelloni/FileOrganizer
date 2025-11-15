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

- **Natural Language Commands:**
  - *Description:* Allow users to interact with FileOrganizer using natural language queries and commands (e.g., "find all photos from last summer", "organize documents by project").
  - *Dependencies:* Local LLM integration (e.g., llama.cpp) or cloud API.
  - *Inspired by:* llama-fs, Organize With AI.

- **Usage Pattern Learning:**
  - *Description:* Track file access patterns and learn user behavior to make intelligent suggestions for organization, archiving, and cleanup.
  - *Inspired by:* Declutr AI.

- **Template-Based Organization:**
  - *Description:* Provide pre-built organization templates for common scenarios (photography projects, software development, documents, music library) that users can apply with customization.
  - *Inspired by:* Folderer, FileFolder.org.

- **Tag-Based Organization:**
  - *Description:* Support user-defined tags for files and folders, with tag-based search, filtering, and organization rules.
  - *Inspired by:* Files, Sigma File Manager.

- **Workspace Management:**
  - *Description:* Allow users to create workspaces for different projects or contexts, each with its own organization rules and settings.
  - *Inspired by:* Sigma File Manager.

- **Timeline View:**
  - *Description:* A chronological view of file activity (created, modified, accessed) to help users understand file evolution and find files by time.
  - *Inspired by:* Sigma File Manager.

- **Dashboard & Statistics:**
  - *Description:* Provide visual dashboards showing file system statistics (storage usage by type, duplicate space savings, organization progress).
  - *Inspired by:* Sigma File Manager.

### 5. Advanced File Operations

- **Batch Renaming with AI:**
  - *Description:* Intelligent batch renaming that suggests names based on file content, metadata, and context.
  - *Dependencies:* Content analysis (OCR, image recognition, document parsing).
  - *Inspired by:* Renamer AI.

- **File Merging & Splitting:**
  - *Description:* Tools to merge multiple files into one or split large files into smaller parts.
  - *Inspired by:* FrankenFile.

- **Format Conversion:**
  - *Description:* Built-in or plugin-based format conversion (image formats, document formats, etc.).
  - *Inspired by:* FrankenFile.

- **Archive Management:**
  - *Description:* Full support for creating, extracting, and browsing archive files (ZIP, RAR, 7-Zip, TAR).
  - *Inspired by:* Open Salamander, Double Commander, Files.

- **File Comparison:**
  - *Description:* Tools to compare files content-wise (text diff, binary diff, image comparison).
  - *Inspired by:* Open Salamander.

### 6. User Interface Enhancements

- **Dual-Pane Mode:**
  - *Description:* Classic dual-pane file manager interface for easier file operations between locations.
  - *Inspired by:* Open Salamander, Double Commander.

- **Tabbed Interface:**
  - *Description:* Support multiple tabs for different locations to reduce window clutter.
  - *Inspired by:* Double Commander, Files, Explorer++.

- **QuickLook/Preview:**
  - *Description:* Quick preview of file contents without opening the full application (images, documents, videos).
  - *Inspired by:* Files, Sigma File Manager.

- **Modern Themes:**
  - *Description:* Customizable themes (light, dark, custom colors) for the GUI.
  - *Inspired by:* Files, Sigma File Manager.

- **Media Gallery View:**
  - *Description:* Specialized view for browsing image and video collections with thumbnails.
  - *Inspired by:* Sigma File Manager.

### 7. Integration & Extensibility

- **Cloud Storage Integration:**
  - *Description:* Support for organizing files in cloud storage (OneDrive, Google Drive, Dropbox, etc.).
  - *Inspired by:* Files, The Drive AI, M-Files.

- **Git Integration:**
  - *Description:* Show git status, integrate with version control operations.
  - *Inspired by:* Files.

- **FTP/FTPS Client:**
  - *Description:* Built-in FTP client for remote file management.
  - *Inspired by:* Open Salamander.

- **Workflow Automation:**
  - *Description:* Advanced workflow engine for automating complex file operations with conditions and triggers.
  - *Inspired by:* M-Files, AI File Pro.

- **Integration APIs:**
  - *Description:* REST API or similar for integration with other tools and scripts.
  - *Inspired by:* FileAI Pro.

### 8. Privacy & Security

- **Encrypted Storage:**
  - *Description:* Support for encrypted containers or folders to protect sensitive files.
  - *Inspired by:* Sigma File Manager.

- **Local-Only Processing:**
  - *Description:* Ensure all AI and processing happens locally without cloud dependency for privacy.
  - *Inspired by:* Local-File-Organizer.

- **Audit Trail:**
  - *Description:* Complete logging of all file operations for compliance and review.
  - *Inspired by:* M-Files.

### 9. Enterprise Features (Post-v1.0, Specialized)

- **Multi-User Collaboration:**
  - *Description:* Support for team workflows with file locking, check-in/check-out.
  - *Inspired by:* M-Files, The Drive AI.

- **Compliance Features:**
  - *Description:* Features for regulatory compliance (retention policies, audit trails, access controls).
  - *Inspired by:* M-Files.

- **Multi-Repository Support:**
  - *Description:* Manage files across multiple repositories (network shares, cloud storage, local drives) from a single interface.
  - *Inspired by:* M-Files.

### 10. Productivity Tools

- **Note-Taking Integration:**
  - *Description:* Integrated note-taking for files and folders (annotations, comments, TODO lists).
  - *Inspired by:* Sigma File Manager.

- **Built-in Viewers:**
  - *Description:* Support for viewing various file types without external applications (text, images, PDFs, code).
  - *Inspired by:* Open Salamander, Double Commander.

- **Internal Text Editor:**
  - *Description:* Basic text editing capabilities for quick edits.
  - *Inspired by:* Double Commander.

- **Keyboard Shortcuts:**
  - *Description:* Comprehensive, customizable keyboard shortcuts for power users.
  - *Inspired by:* Files, Sigma File Manager, Double Commander.
