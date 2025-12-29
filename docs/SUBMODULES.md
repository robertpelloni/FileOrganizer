# Submodule Dashboard

**Version:** 1.4.0  
**Last Updated:** 2025-12-28

## Project Structure

```
FileOrganizer/
├── core/                    # Core library (fo_core)
│   ├── include/fo/core/     # Public headers
│   │   ├── interfaces.hpp   # IFileScanner, IHasher, IMetadataProvider, etc.
│   │   ├── types.hpp        # FileInfo, DateMetadata, DuplicateGroup
│   │   ├── registry.hpp     # Generic plugin registry
│   │   ├── engine.hpp       # Main Engine class
│   │   ├── database.hpp     # DatabaseManager with migrations
│   │   ├── file_repository.hpp
│   │   ├── duplicate_repository.hpp
│   │   ├── operation_repository.hpp  # Undo support
│   │   ├── rule_engine.hpp  # Organization rules
│   │   ├── export.hpp       # JSON/CSV/HTML export
│   │   ├── thumbnail.hpp    # Thumbnail generation
│   │   └── version.hpp      # Auto-generated version
│   └── src/                 # Implementation files
│       ├── scanner_*.cpp    # std, win32, dirent scanners
│       ├── hasher_*.cpp     # fast64, sha256, xxhash, blake3
│       ├── perceptual_opencv.cpp  # dHash, pHash, aHash
│       ├── ocr_tesseract.cpp      # Tesseract OCR
│       ├── classifier_onnx.cpp    # AI classification
│       └── ...
├── cli/                     # Command Line Interface
│   └── fo_cli.cpp           # Main CLI application (15+ commands)
├── gui/                     # Qt6 GUI Application
│   ├── mainwindow.cpp/h     # Main window with tabs
│   └── CMakeLists.txt
├── tests/                   # Unit tests (Google Test)
│   ├── test_export.cpp
│   ├── test_hasher.cpp
│   └── test_scanner.cpp
├── benchmarks/              # Performance benchmarks
│   └── fo_benchmarks.cpp
├── docs/                    # Documentation
│   ├── ROADMAP.md           # Development roadmap
│   ├── SUBMODULES.md        # This file
│   ├── LLM_INSTRUCTIONS.md  # Universal AI instructions
│   └── ...
├── libs/                    # 130+ git submodules (see below)
├── vcpkg/                   # vcpkg package manager
├── VERSION.md               # Single source of truth for version
├── CHANGELOG.md             # Release history
├── AGENTS.md                # AI agent handoff log
├── CMakeLists.txt           # Root CMake configuration
└── build.bat                # Windows build script
```

### Key Components

| Component | Description |
|-----------|-------------|
| **fo_core** | Static library with all core functionality |
| **fo_cli** | CLI executable with 15+ commands |
| **fo_gui** | Qt6 GUI application (optional) |
| **fo_tests** | Unit test executable |
| **fo_benchmarks** | Benchmark executable |

---

## Submodules by Category

### Image/Media Processing
| Name | Path | Version | Commit | URL |
|------|------|---------|--------|-----|
| ImageMagick | `libs/ImageMagick` | 7.1.2-12 | `0db229e52` | [Link](https://github.com/ImageMagick/ImageMagick) |
| OpenImageIO | `libs/OpenImageIO` | v3.0.0.0-beta1 | `ff8965449` | [Link](https://github.com/AcademySoftwareFoundation/OpenImageIO) |
| libheif | `libs/libheif` | v1.20.1+ | `81b09baa` | [Link](https://github.com/strukturag/libheif) |
| heif | `libs/heif` | v3.7.1 | `503194e` | [Link](https://github.com/nokiatech/heif) |
| libvips | `libs/libvips` | v8.17.0 | `9e641793c` | [Link](https://github.com/libvips/libvips) |
| opencv | `libs/opencv` | 4.12.0 | `40ab411032` | [Link](https://github.com/opencv/opencv) |
| FFmpeg | `libs/FFmpeg` | n8.1-dev | `9ab2a437a1` | [Link](https://github.com/FFmpeg/FFmpeg) |
| libpng | `libs/libpng` | 1.6.43 | `44baa877` | [Link](https://github.com/glennrp/libpng) |
| libjpeg-turbo | `libs/libjpeg-turbo` | 2.1.4+ | `af9c1c26` | [Link](https://github.com/libjpeg-turbo/libjpeg-turbo) |
| libwebp | `libs/libwebp` | 1.3.2 | `f6ee4b24` | [Link](https://github.com/webmproject/libwebp) |
| openjpeg | `libs/openjpeg` | v2.5.4 | `8ac52623` | [Link](https://github.com/uclouvain/openjpeg) |
| libavif | `libs/libavif` | 1.1.1 | `ecc52e7c` | [Link](https://github.com/AOMediaCodec/libavif) |

### Metadata/EXIF
| Name | Path | Version | Commit | URL |
|------|------|---------|--------|-----|
| TinyEXIF | `libs/TinyEXIF` | 1.0.2 | `39bba86` | [Link](https://github.com/cdcseacave/TinyEXIF) |
| libexif | `libs/libexif` | 0.6.25 | `cdfe2db` | [Link](https://github.com/libexif/libexif) |
| jhead | `libs/jhead` | 3.08 | `db00efb` | [Link](https://github.com/Matthias-Wandel/jhead) |

### Hashing
| Name | Path | Version | Commit | URL |
|------|------|---------|--------|-----|
| xxHash | `libs/xxHash` | v0.7.4+ | `6697932` | [Link](https://github.com/Cyan4973/xxHash) |
| hash-library | `libs/hash-library` | v8 | `d389d18` | [Link](https://github.com/stbrumme/hash-library) |
| ssdeep | `libs/ssdeep` | 2.13 | `c509f99` | [Link](https://github.com/ssdeep-project/ssdeep) |
| hashcat | `libs/hashcat` | v3.00+ | `2d71af371` | [Link](https://github.com/hashcat/hashcat) |
| argon2 | `libs/argon2` | 20190702 | `fb53ef3` | [Link](https://github.com/P-H-C/phc-winner-argon2) |
| libsodium | `libs/libsodium` | 1.0.20 | `822546c` | [Link](https://github.com/jedisct1/libsodium) |

### Perceptual Hashing
| Name | Path | Version | Commit | URL |
|------|------|---------|--------|-----|
| pHash | `libs/pHash` | - | `3a70447` | [Link](https://github.com/aetiber/pHash) |
| pHash.c | `libs/pHash.c` | v1.1.0 | `1d7c056` | [Link](https://github.com/pHash-org/pHash.c) |
| imagehash | `libs/imagehash` | v1.0.7 | `1a775c3` | [Link](https://github.com/JohannesBuchner/imagehash) |
| tinyphash | `libs/tinyphash` | - | `7c7c503` | [Link](https://github.com/user/tinyphash) |
| perceptual-dct-hash | `libs/perceptual-dct-hash` | - | `9646476` | [Link](https://github.com/user/perceptual-dct-hash) |

### Database
| Name | Path | Version | Commit | URL |
|------|------|---------|--------|-----|
| sqlite | `libs/sqlite` | 3.51.0+ | `6693e78772` | [Link](https://github.com/sqlite/sqlite) |
| sqlite3 | `libs/sqlite3` | 3.49.2 | `182a6a4` | [Link](https://github.com/user/sqlite3) |
| lmdb | `libs/lmdb` | 0.9.33 | `46aef27` | [Link](https://github.com/LMDB/lmdb) |
| postgresql | `libs/postgresql` | 16.9 | `59a67cb` | [Link](https://github.com/postgres/postgres) |
| qdbm | `libs/qdbm` | 1.8.78 | `586aa47` | [Link](https://github.com/fallabs/qdbm) |

### Compression
| Name | Path | Version | Commit | URL |
|------|------|---------|--------|-----|
| zlib | `libs/zlib` | 1.3.1 | `65dd212` | [Link](https://github.com/madler/zlib) |
| libbzip2 | `libs/libbzip2` | 1.0.8 | `99d3c48` | [Link](https://github.com/user/libbzip2) |
| libzip | `libs/libzip` | v1.11.4 | `a1364389` | [Link](https://github.com/nih-at/libzip) |
| fast-lzma2 | `libs/fast-lzma2` | v1.0.1 | `b44b79b` | [Link](https://github.com/conor42/fast-lzma2) |
| pxz | `libs/pxz` | - | `136e5c2` | [Link](https://github.com/jnovy/pxz) |
| pngquant | `libs/pngquant` | 3.0.3 | `e3bdc7c` | [Link](https://github.com/kornelski/pngquant) |

### Networking/HTTP
| Name | Path | Version | Commit | URL |
|------|------|---------|--------|-----|
| cURL | `libs/cURL` | 8.17.0 | `97a22cc` | [Link](https://github.com/curl/curl) |
| openssl | `libs/openssl` | 1.1.1t | `58bea78` | [Link](https://github.com/openssl/openssl) |
| libssh2 | `libs/libssh2` | 1.11.1 | `a7685495` | [Link](https://github.com/libssh2/libssh2) |
| nghttp2 | `libs/nghttp2` | 1.64.0 | `2bfeb83` | [Link](https://github.com/nghttp2/nghttp2) |
| c-ares | `libs/c-ares` | - | `6c783a9` | [Link](https://github.com/c-ares/c-ares) |
| libevent | `libs/libevent` | 2.2.1-alpha | `a994a52d` | [Link](https://github.com/libevent/libevent) |

### Windows-Specific (ADS/NTFS)
| Name | Path | Version | Commit | URL |
|------|------|---------|--------|-----|
| ADSFileSystem | `libs/ADSFileSystem` | - | `60b6710` | [Link](https://github.com/user/ADSFileSystem) |
| ADSIdentifier | `libs/ADSIdentifier` | - | `099e808` | [Link](https://github.com/user/ADSIdentifier) |
| ADSman | `libs/ADSman` | - | `187e647` | [Link](https://github.com/user/ADSman) |
| AlternateDataStreams | `libs/AlternateDataStreams` | - | `34cd1cb` | [Link](https://github.com/user/AlternateDataStreams) |
| DataStreamBrowser | `libs/DataStreamBrowser` | - | `88db018` | [Link](https://github.com/user/DataStreamBrowser) |
| lsads | `libs/lsads` | - | `e2d1599` | [Link](https://github.com/user/lsads) |

### Academy Software Foundation
| Name | Path | Version | Commit | URL |
|------|------|---------|--------|-----|
| Imath | `libs/Imath` | v3.2.0 | `4de9a1d` | [Link](https://github.com/AcademySoftwareFoundation/Imath) |
| openexr | `libs/openexr` | v3.4-alpha | `166bc483` | [Link](https://github.com/AcademySoftwareFoundation/openexr) |
| OpenColorIO | `libs/OpenColorIO` | v2.1.0 | `1d77ecda` | [Link](https://github.com/AcademySoftwareFoundation/OpenColorIO) |
| OpenTimelineIO | `libs/OpenTimelineIO` | v0.18.1 | `4af7407` | [Link](https://github.com/AcademySoftwareFoundation/OpenTimelineIO) |
| OpenCue | `libs/OpenCue` | v1.13.8 | `9d7fafcd` | [Link](https://github.com/AcademySoftwareFoundation/OpenCue) |
| OpenRV | `libs/OpenRV` | v3.0.0 | `7351285` | [Link](https://github.com/AcademySoftwareFoundation/OpenRV) |
| openfx | `libs/openfx` | 1.5.1 | `ab77951` | [Link](https://github.com/AcademySoftwareFoundation/openfx) |
| openapv | `libs/openapv` | v0.2.0.4 | `1a7845a` | [Link](https://github.com/AcademySoftwareFoundation/openapv) |

### Media Players/Codecs
| Name | Path | Version | Commit | URL |
|------|------|---------|--------|-----|
| mpv | `libs/mpv` | v0.41.0+ | `4ecf7293f5` | [Link](https://github.com/mpv-player/mpv) |
| vlc | `libs/vlc` | 4.0.0-dev | `5a666ee2df` | [Link](https://github.com/videolan/vlc) |
| openh264 | `libs/openh264` | v2.6.0 | `cf568c83` | [Link](https://github.com/cisco/openh264) |
| libvpx | `libs/libvpx` | 1.1.0 | `5e76714` | [Link](https://github.com/webmproject/libvpx) |

### Utilities
| Name | Path | Version | Commit | URL |
|------|------|---------|--------|-----|
| dirent | `libs/dirent` | - | `f7d4841` | [Link](https://github.com/tronkko/dirent) |
| getopt-win | `libs/getopt-win` | v2.42.0 | `36e55dd` | [Link](https://github.com/user/getopt-win) |
| libunistd | `libs/libunistd` | v1.2 | `18b4e78` | [Link](https://github.com/user/libunistd) |
| tinyxml2 | `libs/tinyxml2` | 2.0.2+ | `3324d04` | [Link](https://github.com/leethomason/tinyxml2) |
| json-c | `libs/json-c` | - | `a1249bf` | [Link](https://github.com/json-c/json-c) |
| libxml2 | `libs/libxml2` | v2.15.0 | `9827e6e4` | [Link](https://github.com/GNOME/libxml2) |

### Audio Recognition
| Name | Path | Version | Commit | URL |
|------|------|---------|--------|-----|
| ShazamIO | `libs/ShazamIO` | 0.8.1 | `b5321b5` | [Link](https://github.com/user/ShazamIO) |
| ShazamAPI | `libs/ShazamAPI` | - | `ff8f89d` | [Link](https://github.com/user/ShazamAPI) |
| audio-recognizer | `libs/audio-recognizer` | - | `3028b61` | [Link](https://github.com/user/audio-recognizer) |
| audiocraft | `libs/audiocraft` | v1.3.0 | `896ec7c` | [Link](https://github.com/facebookresearch/audiocraft) |

### Document Processing
| Name | Path | Version | Commit | URL |
|------|------|---------|--------|-----|
| sumatrapdf | `libs/sumatrapdf` | 3.1.2+ | `3e74038b3` | [Link](https://github.com/sumatrapdfreader/sumatrapdf) |
| pslib | `libs/pslib` | 0.4.6 | `4c6b93b` | [Link](https://github.com/user/pslib) |
| libtidy | `libs/libtidy` | 5.6.0 | `cfff694` | [Link](https://github.com/htacg/tidy-html5) |

### GUI/Graphics
| Name | Path | Version | Commit | URL |
|------|------|---------|--------|-----|
| raylib | `libs/raylib` | 5.5+ | `8f834604` | [Link](https://github.com/raysan5/raylib) |
| lvgl | `libs/lvgl` | v9.3.0 | `18354afc0` | [Link](https://github.com/lvgl/lvgl) |
| freetype | `libs/freetype` | 2.13.3 | `70391bfb` | [Link](https://github.com/freetype/freetype) |

### Reverse Engineering
| Name | Path | Version | Commit | URL |
|------|------|---------|--------|-----|
| radare2 | `libs/radare2` | 6.0.7+ | `291c5ff708` | [Link](https://github.com/radareorg/radare2) |

### Package Manager
| Name | Path | Version | Commit | URL |
|------|------|---------|--------|-----|
| vcpkg | `vcpkg` | 2022.02.02+ | `11bbc873e0` | [Link](https://github.com/microsoft/vcpkg) |

---

## Submodule Statistics

- **Total Submodules:** 130+
- **Categories:** 15
- **Last Full Update:** 2025-12-28
- **Build Status:** ✅ Windows MSVC verified

---

## Updating Submodules

```powershell
# Initialize and update all submodules
git submodule update --init --recursive

# Update to latest upstream commits
git submodule foreach "git fetch origin && git checkout origin/HEAD 2>/dev/null || git checkout origin/master 2>/dev/null || true"

# View submodule status
git submodule status
```

---

## Notes

- Some submodules have upstream changes available (marked with `+` in `git submodule status`)
- Not all submodules are actively used; many are reference implementations or for future features
- vcpkg is used for optional dependencies (Tesseract, BLAKE3, Exiv2, OpenCV, ONNX Runtime)
- Core functionality uses vendored libs: TinyEXIF, hash-library, xxHash, sqlite3
