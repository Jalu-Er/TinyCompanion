# Changelog

All notable changes to the **TinyCompanion** project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [Unreleased]

### Added
*   Complete project layout skeleton for PlatformIO configuration.
*   Pure C++ abstract Hardware Abstraction Layer (HAL) contracts for all target inputs and outputs.
*   Event circular buffer structures and configuration parameters.
*   Core logic headers (`StateMachine`, `EmotionEngine`, `PersonalityEngine`, `AnimationEngine`).
*   Project documents: `SOFTWARE_DESIGN_DOCUMENT.md`, `ROADMAP.md`, `MILESTONES.md`, `CONTRIBUTING.md`, `ARCHITECTURE.md`.
*   Standard license file (MIT) and git ignore files.

### Changed
*   Not applicable (Initial project setup).

### Fixed
*   Not applicable (Initial project setup).

### Known Issues
*   The project contains only mock declarations and interface wrappers; there is no functional firmware implementation yet.
*   Compilation configuration targeting Arduino Uno requires installing driver libraries (Wire, Adafruit SSD1306/SH1106, etc.) which will be managed in Phase 2.
