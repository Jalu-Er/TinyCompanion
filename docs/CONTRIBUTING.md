# Contributing to TinyCompanion

Thank you for your interest in contributing to the **TinyCompanion** project! This document outlines the standards, conventions, and procedures we follow to maintain high code quality and architecture clean.

---

## 1. Coding Style (C++ Standards)
This project follows a strict embedded coding style based on **Google C++ Style Guide** with optimizations for resource-constrained 8-bit systems.

### General Rules:
*   **Zero Dynamic Memory Allocation:** Do not use `new`, `delete`, `malloc()`, `free()`, `realloc()`, or dynamic classes like `std::vector` or `std::string` after the startup phase. All structures must use statically-sized arrays or fixed ring-buffers.
*   **No Blocking Calls:** Never call `delay()`. Use cooperative task structures using non-blocking timers comparing timestamps (`millis()`).
*   **Naming Conventions:**
    *   **Classes/Structs:** PascalCase (e.g., `SensorManager`, `TimeStruct`).
    *   **Interfaces:** PascalCase prefixed with 'I' (e.g., `IUltrasonic`, `IRtcClock`).
    *   **Methods/Functions:** camelCase (e.g., `getDistanceCm()`, `processEvent()`).
    *   **Variables:** camelCase (e.g., `excitability`, `lastRunMs`).
    *   **Constants/Macros/Enums:** UPPER_SNAKE_CASE (e.g., `LDR_DARK_THRESHOLD`, `EventType::TOUCH_TRIGGERED`).
*   **Formatting:** Indentation must be **4 spaces**. Do not use tabs. Put braces `{}` on the same line as statements.
*   **Header Guards:** Use `#pragma once` at the very top of header files.

---

## 2. Commit Convention
We follow the **Conventional Commits** specification:

`<type>(<scope>): <description>`

### Allowed Types:
*   `feat`: A new feature (e.g., adding a driver).
*   `fix`: A bug fix.
*   `docs`: Documentation changes.
*   `style`: Code style modifications (formatting, white-spaces, missing semi-colons).
*   `refactor`: Code restructuring that does not fix bugs or add features.
*   `test`: Adding or correcting tests.
*   `chore`: Miscellaneous tasks (PlatformIO config updates, library version bumps).

### Examples:
*   `feat(hal): implement ArduinoTouch capacitive driver adapter`
*   `fix(emotion): correct valence decay calculation overflow`
*   `docs(architecture): update Mermaid dependency diagram layout`

---

## 3. Branch Naming
Keep branch names descriptive and short, using hyphens:

*   `feature/<short-description>` - For implementing new features.
*   `bugfix/<short-description>` - For fixing issues.
*   `refactor/<short-description>` - For cleanup.
*   `docs/<short-description>` - For writing manuals or specifications.

### Examples:
*   `feature/add-oled-procedural-eye`
*   `bugfix/resolve-median-filter-lag`

---

## 4. Pull Request (PR) Process
1.  **Fork and Branch:** Create a branch from `main` to start your work.
2.  **Ensure Code Quality:** Validate that the firmware compiles cleanly on PlatformIO target environments (Uno and/or ESP32).
3.  **Update Changelog:** Add a line summarizing your changes under the `[Unreleased]` section of `CHANGELOG.md`.
4.  **Submit PR:** Open a Pull Request targeting the `main` branch. Provide a detailed description of what you did and attach any serial logs or diagrams.

---

## 5. Code Review Checklist
Before marking your PR as ready for review, check:
*   [ ] Does the code compile without any warnings?
*   [ ] Does the code contain any `delay()` statements?
*   [ ] Does the code perform any dynamic allocations (`malloc`, `new`, `String`)?
*   [ ] Are all variables correctly scoped and constants placed in flash memory via `PROGMEM`?
*   [ ] Are the file links correct and do all classes match their responsibilities defined in `SOFTWARE_DESIGN_DOCUMENT.md`?

---

## 6. Documentation Rules
*   Every new class and interface must be fully documented using Doxygen/Javadoc-style header comments.
*   Update `ARCHITECTURE.md` if any structural changes (new modules, new events, altered data flow) are introduced.
*   All documents must use absolute file links rather than backticks for links to simplify repository navigation.
