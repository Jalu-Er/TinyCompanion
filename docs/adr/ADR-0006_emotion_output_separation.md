# ADR-0006: Emotion Output Separation

*   **Status:** Accepted
*   **Date:** 2026-08-01

## Context
TinyCompanion communicates its emotional state (Valence and Arousal) through multiple physical channels: facial graphics, ambient lighting, melodic sound effects, and diagnostic clock segments. Coupling these physical displays directly into a single massive output manager would create a fragile system that is hard to maintain, debug, and test.

## Decision
Enforce a strict separation of emotion output responsibilities across hardware channels:
*   **OLED Display (Wajah / Wajah Utama):** Responsible strictly for micro-expressions, facial rendering (pupils scaling, eye blinking positions, look angles). It represents the companion's direct target point of focus.
*   **RGB LED Aura (Ambient Emotional Lighting):** Responsible for broad environmental feeling feedback (e.g. glowing soft green for safety/idle, pulsing cyan for happiness, flashing red for system errors). This provides immediate status readability from a distance.
*   **Buzzer (Audio Feedback):** Responsible for short, non-blocking melodic tones and frequency chirps matching emotional mood swings.
*   **TM1637 Segment (Secondary Info / Clock):** Reserved exclusively for time synchronization status and diagnostic numbers (e.g. system uptime, sensor read debugs) so as not to distract from the main OLED face.

```
                  +-----------------------------------+
                  |      Russell Emotion Engine       |
                  +-----------------------------------+
                                    │
                  ┌─────────────────┼─────────────────┐
                  ▼                 ▼                 ▼
        +------------------+ +-------------+ +----------------+
        |   EyeRenderer    | |   LedAura   | |  AudioChirper  |
        +------------------+ +-------------+ +----------------+
        | OLED Display     | | RGB LED     | | Passive Buzzer |
        | (Face/Focus)     | | (Ambient)   | | (Audio SFX)    |
        +------------------+ +-------------+ +----------------+
```

## Alternatives Considered
*   **Monolithic UI Manager:** A single class `UiManager` that calls `display()`, `digitalWrite(LED)`, and `tone()` together. This was rejected because it causes high coupling, making it very difficult to swap out one hardware piece (e.g. switching Buzzer to DAC speaker) without breaking the display layout.

## Consequences
*   **Pros:** 
    *   High maintainability: You can change the RGB LED layout or rewrite the OLED drawing algorithms independently.
    *   Clean unit testing: Each output adapter has its own generic test interface (`IValidation`), ensuring easy isolated test benches.
*   **Cons:** 
    *   Requires a central orchestration layer (e.g., future behavior engines or event managers) to dispatch coordinated emotional changes to all channels concurrently.

## Future Revisions
None planned.
