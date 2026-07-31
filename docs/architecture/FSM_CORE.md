# Finite State Machine (FSM) Core Architecture

This document describes the design and transition logic of the behavioral state engine of **TinyCompanion**.

---

## 1. Behavioral State Transitions

The FSM acts as the single orchestrator of companion behavioral states:

```
                  [ BOOT ]
                     │ (INITIALIZATION_FINISHED)
                     ▼
                 ┌───►[ IDLE ]◄──────────────────┐
                 │    │  │  │                     │
                 │    │  │  └────────────────┐    │ (TOUCH_PRESSED)
(USER_LEAVING)   │    │  │ (AMBIENT_DARK)    │    │
                 │    │  │                   ▼    │
                 │    │  ▼             [ SLEEPING ]
                 │    │ [ OBSERVING ]         ▲
                 │    │  │   ▲                │ (AMBIENT_DARK)
                 │    │  │   │                │
                 │    ▼  ▼   │ (TOUCH_PRESSED)│
                 └──[ HAPPY ]┘────────────────┘
```

---

## 2. Structural Patterns

To keep logic scalable and avoid giant switch blocks, we use the C++ **State Pattern**:
*   **`IState` (Abstract Interface):** Declares lifecycle callbacks: `onEnter()`, `onExit()`, `handleEvent()`, and `tick()`.
*   **`StateContext` (Abstract Context):** Decouples state implementations from the concrete StateMachine, exposing a single `transitionTo()` hook.
*   **`StateMachine` (Concrete Context):** Houses the active state instance pointer and broadcasts `STATE_CHANGED` events to the external Event Pipeline during transitions.
*   **Static Allocation:** Concrete state instances are allocated statically inside `StateMachine.cpp`, avoiding dynamic memory `new` calls to prevent heap fragmentation.
