# Universal LLM Instructions

This document serves as the central instruction file for all AI agents (GitHub Copilot, Claude, Gemini, GPT, etc.) working on the FileOrganizer project.

## 1. Core Principles

- **Single Source of Truth**: Always check `VERSION.md` for the current project version.
- **Changelog Updates**: Every significant change must be recorded in `CHANGELOG.md`.
- **Documentation First**: Update documentation (`docs/`) alongside code changes.
- **Submodule Awareness**: Be aware of the extensive list of submodules in `libs/`. Refer to `docs/SUBMODULES.md`.

## 2. Versioning Protocol

- The project version is stored in `VERSION.md` (e.g., `0.2.0`).
- When releasing or making major changes:
    1.  Increment the version in `VERSION.md`.
    2.  Add a new entry in `CHANGELOG.md`.
    3.  Commit with a message like `chore: bump version to 0.2.0`.

## 3. Coding Standards

- **C++ Standard**: C++20.
- **Platform**: Cross-platform (Windows, Linux, macOS), with a preference for native Windows APIs (`win32`) where performance is critical.
- **Style**: Follow existing conventions (snake_case for variables/functions, CamelCase for classes).

## 4. Handoff Protocol

- When finishing a session, update `AGENTS.md` with a "Handoff Log" entry.
- Include:
    - Date and Author.
    - Scope of work.
    - Current Status.
    - Next Steps.

## 5. Model-Specific Instructions

### GitHub Copilot
- Use the provided tools efficiently.
- Prefer `run_in_terminal` for git operations and builds.

### Claude / Anthropic
- Focus on architectural analysis and detailed documentation.

### Gemini / Google
- Leverage large context window for deep code analysis.

### GPT / OpenAI
- Excellent at code generation and refactoring.
