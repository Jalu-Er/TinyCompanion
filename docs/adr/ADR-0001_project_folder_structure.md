# ADR-0001: Project Folder Structure

*   **Status:** Accepted
*   **Date:** 2026-08-01

## Context
TinyCompanion is a multi-disciplinary hardware-software project. It includes C++ source code (firmware), mechanical structures (3D CAD designs), and schematic layouts (electronics). A flat or chaotic folder organization would make collaboration difficult, increase build tool coupling, and hinder repository maintenance.

## Decision
Adopt a structured top-level layout that segregates the project by domains:
*   `docs/`: Documentations (ADRs, test guidelines, specs).
*   `firmware/`: All compilation targets, library layers, and PlatformIO files.
*   `hardware/`: Electronic KiCAD schematics and 3D printed mechanical enclosure files.
*   `assets/`: Media resources (eyes designs, sound frequencies, sheets).
*   `tools/`: Command-line scripts for automation and conversion.

## Alternatives Considered
*   **PlatformIO Root Layout:** Putting PlatformIO files in the repository root. This was rejected because it mixes C++ compiler dependencies with mechanical and electronic drawings, cluttering the root folder.

## Consequences
*   **Pros:** Clean domain segregation. Easy for electronic or mechanical contributors to navigate.
*   **Cons:** Developers must open the `firmware/` subfolder in VS Code to trigger the PlatformIO build system seamlessly.

## Future Revisions
None planned. The structure is sufficient for the target project scale.
