# ADR-0007: Semantic Event Architecture

*   **Status:** Accepted
*   **Date:** 2026-08-01

## Context
High-level behavior modules (personality decay, mood states, animation reactions) need to know when environmental conditions change (e.g. the user comes near, a room gets dark, or touch is pressed). Coupling these modules directly to raw analog ADC values or ultrasonic distance centimeters causes significant architecture issues:
1.  Changes to physical thresholds require modifying business logic rules.
2.  Noise spikes from sensors (like ultrasonic ripples) cause jitter in high-level state decisions.
3.  Simulating hardware behavior under automated software testing becomes extremely difficult.

## Decision
Introduce a **Semantic Event-Driven Architecture**:
*   All high-level application modules depend strictly on discrete, semantic events pushed to a central `EventQueue` (e.g. `TOUCH_PRESSED`, `USER_APPROACHING`, `AMBIENT_DARK`).
*   No application module may poll hardware pins directly or access metric outputs (such as ADC numbers or raw centimeters).
*   Sensor processing, noise smoothing, and transition thresholds are encapsulated entirely inside the `SensorManager` (and the concrete driver adapter layer).

### Concrete Example:
*   *Tight coupling (Bad):*
    ```cpp
    void EmotionEngine::update() {
        uint16_t light = analogRead(A0);
        if (light > 750) {
            setMood(Mood::Scared);
        }
    }
    ```
*   *Decoupled Events (Good):*
    ```cpp
    void EmotionEngine::handleEvent(const Event& event) {
        if (event.type == EventType::AMBIENT_DARK) {
            setMood(Mood::Scared);
        }
    }
    ```

## Alternatives Considered
*   **Direct Metric Polling:** Exposing clean metric getters (e.g. `getDistanceCm()`, `getLightLux()`) to the behavioral engine. This was rejected because the engine would still have to run custom polling logic, determine thresholds, and maintain state records for transitions.

## Consequences
*   **Pros:**
    *   **Loose Coupling:** The personality or emotion engine does not care how distance or light is measured. We can swap LDR with an SPI light chip, and the high-level logic remains completely unchanged.
    *   **Automated Testing:** We can easily simulate touch, distance, or lighting events during unit testing by pushes directly to the `EventQueue` without needing any physical sensors.
*   **Cons:**
    *   Adds a small RAM/Flash footprint due to queue buffer indexing and event classification tables.

## Future Revisions
None.
