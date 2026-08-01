# Emotion Engine and Expression Model Architecture

This document describes the design, dependency paths, and emotional update loops of the continuous presentation layer in **TinyCompanion**.

---

## 1. Dependency Flow

To maintain code portability and keep logic decoupled, we enforce the **Dependency Inversion Principle**. Presentation layers are decoupled into business logic variables (`EmotionEngine`) and abstract presentation mappings (`ExpressionEngine`):

```
[Sensors] ──► [Event Queue] ──► [FSM] ──► [EmotionEngine] ──► [ExpressionEngine]
                                                                     │
                                                                     ▼
[Output Interfaces] ◄─────────────────────────────────────── [Expression Struct]
(IOledDisplay, ILedAura, IBuzzer)
        ▲
        │ (Implemented by concrete classes)
[Concrete Drivers]
(ArduinoOled, ArduinoLed, etc.)
```

### Dependency Rules:
1.  **Strict Isolation:** `EmotionEngine` and `ExpressionEngine` have zero imports of hardware drivers, Arduino APIs, or graphics libraries. They contain purely continuous mathematical calculations.
2.  **Abstract Interfacing:** Managers (like `LedManager` or `DisplayManager`) do not perform behavioral decisions. They receive read-only `Expression` structs and map the abstract geometry (e.g. squint eyes, happy blinking) directly to registers.
3.  **Renderer Decoupling:** The future OLED Eye Renderer will draw shapes matching the abstract `Expression` state, remaining entirely oblivious to sensors, FSM zones, or Valence-Arousal values.

---

## 2. Emotional Update Loops

The continuous emotional variables space maps Russell's 2D plane as follows:
*   **Valence:** Measures pleasantness (positive vs. negative). Pushed positively by touch and negatively by isolation.
*   **Arousal:** Measures activation (high energy vs. sleepiness). Boosted by proximity alerts and touch.
*   **Trust, Curiosity, Fatigue:** Auxiliary states that track time and event frequency.

### Periodic Logic Update Ticks:
Every 100 ms (`TICK_PERIOD_LOGIC`), the FSM ticks and the EmotionEngine decays the valence and arousal scales towards 0 (neutral). ExpressionEngine calculates a new `Expression` struct, which is immediately pushed to the managers.
