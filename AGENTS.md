# AGENTS.md

> For detailed handoff log and full guidelines, see [docs/LLM_INSTRUCTIONS.md](docs/LLM_INSTRUCTIONS.md)

## Build Commands
```powershell
build.bat                                    # Quick build (Windows)
cmake -S . -B build -G Ninja && cmake --build build  # Manual build
.\build\tests\fo_tests.exe                   # Run all tests
.\build\tests\fo_tests.exe --gtest_filter=*Name*     # Run single test
.\build\cli\fo_cli.exe --help                # CLI usage
```

## Architecture
- **core/** - `fo_core` static library (interfaces, providers, engine, database)
- **cli/** - `fo_cli` executable (primary user interface)
- **libs/** - 100+ git submodules (TinyEXIF, xxHash, sqlite3, etc.)
- **Registry pattern**: Providers (scanners, hashers, metadata, OCR) registered via `Registry<T>::instance().add()`

## Code Style
- **C++20** with `std::filesystem`, `std::chrono`, `std::optional`
- **Naming**: `snake_case` for functions/variables, `CamelCase` for classes
- **Headers**: `.hpp` extension, `#pragma once`, group includes (std → external → internal)
- **Platform code**: Guard with `#ifdef _WIN32`, provide portable defaults
- **Optional deps**: Guard with `FO_HAVE_*` defines (e.g., `FO_HAVE_TESSERACT`)
- **Commits**: `feat:`, `fix:`, `chore:`, `docs:`, `refactor:`, `test:`
