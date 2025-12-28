# Universal LLM Instructions

This document serves as the central instruction file for all AI agents (GitHub Copilot, Claude, Gemini, GPT, etc.) working on the FileOrganizer project.

**Current Version:** See `VERSION.md` (Single Source of Truth)

## 1. Core Principles

- **Single Source of Truth**: `VERSION.md` contains the version string only.
- **Changelog Updates**: Every significant change must be recorded in `CHANGELOG.md`.
- **Documentation First**: Update documentation (`docs/`) alongside code changes.
- **Submodule Awareness**: 100+ submodules in `libs/`. Refer to `docs/SUBMODULES.md`.
- **Autonomy**: Proceed with implementation. Don't stop for confirmation unless blocked.
- **No Regressions**: Verify existing features work after changes.

## 2. Versioning Protocol

### Version Number Format
- Format: `MAJOR.MINOR.PATCH` (e.g., `1.0.0`)
- `VERSION.md` contains ONLY the version string (no other content).

### Version Update Procedure
1. Update `VERSION.md` with new version.
2. Update `CHANGELOG.md` with new entry at top.
3. Update `vcpkg.json` version-string (if applicable).
4. Commit with message: `chore: bump version to X.Y.Z`
5. Push to remote.

### When to Increment
- **MAJOR**: Breaking changes, major feature releases.
- **MINOR**: New features, significant enhancements.
- **PATCH**: Bug fixes, minor improvements.

## 3. Coding Standards

### C++ Guidelines
- **Standard**: C++20 (`std::filesystem`, `std::chrono`, `std::optional`)
- **Headers**: Use `.hpp` extension, guard with `#pragma once`
- **Naming**: `snake_case` for variables/functions, `CamelCase` for classes
- **Includes**: Group by standard/external/internal, alphabetically

### Build System
- **CMake**: Minimum 3.21, use modern targets
- **Generator**: Ninja preferred
- **Dependencies**: vcpkg for external packages
- **Optional Features**: Guard with `FO_HAVE_*` defines

### Registry Pattern
```cpp
// Register a provider in static initialization
static auto reg = []() {
    Registry<IFileScanner>::instance().add("name", []() {
        return std::make_unique<MyScannerImpl>();
    });
    return true;
}();
```

## 4. Project Structure

```
FileOrganizer/
├── core/                    # fo_core static library
│   ├── include/fo/core/     # Public headers
│   └── src/                 # Implementation
├── cli/                     # fo_cli executable
├── gui/                     # fo_gui Qt6 application
├── tests/                   # Unit tests (GTest)
├── benchmarks/              # Performance benchmarks
├── docs/                    # Documentation
├── libs/                    # 100+ git submodules
├── vcpkg/                   # Package manager
├── VERSION.md               # Version (single line)
├── CHANGELOG.md             # Release history
└── AGENTS.md                # AI handoff log
```

## 5. Key Files Reference

| File | Purpose |
|------|---------|
| `VERSION.md` | Version string only |
| `CHANGELOG.md` | Release history with dates |
| `AGENTS.md` | AI session handoff log |
| `docs/ROADMAP.md` | Feature roadmap |
| `docs/SUBMODULES.md` | Submodule dashboard |
| `docs/LLM_INSTRUCTIONS.md` | This file |
| `vcpkg.json` | Package dependencies |

## 6. Handoff Protocol

When finishing a session, update `AGENTS.md`:

```markdown
### Update: YYYY-MM-DD (Session N)

**Author:** [Model Name]

**Scope:** [Brief description]

**Current Status:**
- [Bullet points of completed work]

**Next Steps:**
1. [Numbered list of recommendations]

**Handoff Note:**
[Summary for next AI agent]
```

## 7. Git Workflow

### Commit Messages
- `feat:` New feature
- `fix:` Bug fix
- `chore:` Maintenance (version bump, deps)
- `docs:` Documentation only
- `refactor:` Code restructuring
- `test:` Test additions/changes

### Branch Strategy
- `main`: Stable, deployable
- `feature/*`: Feature development
- Always merge to main when stable

## 8. Model-Specific Instructions

### GitHub Copilot
- Use terminal for git operations and builds.
- Prefer `run_in_terminal` tool.

### Claude / Anthropic
- Focus on architectural analysis.
- Excellent at documentation.

### Gemini / Google
- Leverage large context window.
- Good at comprehensive analysis.

### GPT / OpenAI
- Strong at code generation.
- Good at refactoring.

## 9. Common Tasks

### Build the Project (Windows)
```batch
build.bat
```
Or manually:
```powershell
cmd /c "call vcvars64.bat && cmake -S . -B build -G Ninja && cmake --build build"
```

### Run Tests
```powershell
.\build\tests\fo_tests.exe
```

### Update Submodules
```powershell
git submodule update --init --recursive
```

