/**
 * @file StateMachine.cpp
 * @brief Implementations of high-level FSM and concrete state definitions.
 */

#include "StateMachine.h"
#include "IState.h"

// --- Concrete State Classes Definition ---

class BootState : public IState {
public:
    void onEnter(StateContext& ctx) override { (void)ctx; }
    void onExit(StateContext& ctx) override { (void)ctx; }
    
    void handleEvent(StateContext& ctx, const Event& event) override {
        if (event.type == EventType::INITIALIZATION_FINISHED) {
            ctx.transitionTo(CompanionState::IDLE);
        }
    }
    
    void tick(StateContext& ctx, uint32_t dtMs) override {
        (void)ctx;
        (void)dtMs;
    }
    
    CompanionState getStateId() const override { return CompanionState::BOOT; }
};

class IdleState : public IState {
public:
    void onEnter(StateContext& ctx) override { (void)ctx; }
    void onExit(StateContext& ctx) override { (void)ctx; }
    
    void handleEvent(StateContext& ctx, const Event& event) override {
        switch (event.type) {
            case EventType::TOUCH_PRESSED:
                ctx.transitionTo(CompanionState::HAPPY);
                break;
            case EventType::USER_APPROACHING:
                ctx.transitionTo(CompanionState::OBSERVING);
                break;
            case EventType::AMBIENT_DARK:
                ctx.transitionTo(CompanionState::SLEEPING);
                break;
            case EventType::TIME_PERIOD_AFTERNOON:
                ctx.transitionTo(CompanionState::THINKING);
                break;
            case EventType::TIME_PERIOD_EVENING:
                ctx.transitionTo(CompanionState::ALERT);
                break;
            default:
                break;
        }
    }
    
    void tick(StateContext& ctx, uint32_t dtMs) override {
        (void)ctx;
        (void)dtMs;
    }
    
    CompanionState getStateId() const override { return CompanionState::IDLE; }
};

class SleepingState : public IState {
public:
    void onEnter(StateContext& ctx) override { (void)ctx; }
    void onExit(StateContext& ctx) override { (void)ctx; }
    
    void handleEvent(StateContext& ctx, const Event& event) override {
        if (event.type == EventType::TOUCH_PRESSED || event.type == EventType::AMBIENT_BRIGHT) {
            ctx.transitionTo(CompanionState::IDLE);
        }
    }
    
    void tick(StateContext& ctx, uint32_t dtMs) override {
        (void)ctx;
        (void)dtMs;
    }
    
    CompanionState getStateId() const override { return CompanionState::SLEEPING; }
};

class ObservingState : public IState {
public:
    void onEnter(StateContext& ctx) override { (void)ctx; }
    void onExit(StateContext& ctx) override { (void)ctx; }
    
    void handleEvent(StateContext& ctx, const Event& event) override {
        switch (event.type) {
            case EventType::USER_LEAVING:
                ctx.transitionTo(CompanionState::IDLE);
                break;
            case EventType::TOUCH_PRESSED:
                ctx.transitionTo(CompanionState::HAPPY);
                break;
            case EventType::AMBIENT_DARK:
                ctx.transitionTo(CompanionState::SLEEPING);
                break;
            default:
                break;
        }
    }
    
    void tick(StateContext& ctx, uint32_t dtMs) override {
        (void)ctx;
        (void)dtMs;
    }
    
    CompanionState getStateId() const override { return CompanionState::OBSERVING; }
};

class HappyState : public IState {
private:
    uint32_t timerMs = 0;

public:
    void onEnter(StateContext& ctx) override {
        (void)ctx;
        timerMs = 0;
    }
    
    void onExit(StateContext& ctx) override { (void)ctx; }
    
    void handleEvent(StateContext& ctx, const Event& event) override {
        (void)ctx;
        if (event.type == EventType::TOUCH_PRESSED) {
            timerMs = 0; // Reset happy duration on double tap
        }
    }
    
    void tick(StateContext& ctx, uint32_t dtMs) override {
        timerMs += dtMs;
        if (timerMs >= 2000) {
            ctx.transitionTo(CompanionState::IDLE);
        }
    }
    
    CompanionState getStateId() const override { return CompanionState::HAPPY; }
};

class ThinkingState : public IState {
private:
    uint32_t timerMs = 0;

public:
    void onEnter(StateContext& ctx) override {
        (void)ctx;
        timerMs = 0;
    }
    
    void onExit(StateContext& ctx) override { (void)ctx; }
    
    void handleEvent(StateContext& ctx, const Event& event) override {
        if (event.type == EventType::TOUCH_PRESSED) {
            ctx.transitionTo(CompanionState::HAPPY);
        } else if (event.type == EventType::USER_LEAVING) {
            ctx.transitionTo(CompanionState::IDLE);
        }
    }
    
    void tick(StateContext& ctx, uint32_t dtMs) override {
        timerMs += dtMs;
        if (timerMs >= 5000) {
            ctx.transitionTo(CompanionState::IDLE);
        }
    }
    
    CompanionState getStateId() const override { return CompanionState::THINKING; }
};

class AlertState : public IState {
public:
    void onEnter(StateContext& ctx) override { (void)ctx; }
    void onExit(StateContext& ctx) override { (void)ctx; }
    
    void handleEvent(StateContext& ctx, const Event& event) override {
        if (event.type == EventType::TOUCH_PRESSED) {
            ctx.transitionTo(CompanionState::HAPPY);
        } else if (event.type == EventType::USER_LEAVING) {
            ctx.transitionTo(CompanionState::IDLE);
        }
    }
    
    void tick(StateContext& ctx, uint32_t dtMs) override {
        (void)ctx;
        (void)dtMs;
    }
    
    CompanionState getStateId() const override { return CompanionState::ALERT; }
};

class ErrorState : public IState {
public:
    void onEnter(StateContext& ctx) override { (void)ctx; }
    void onExit(StateContext& ctx) override { (void)ctx; }
    
    void handleEvent(StateContext& ctx, const Event& event) override {
        (void)ctx;
        (void)event;
    }
    
    void tick(StateContext& ctx, uint32_t dtMs) override {
        (void)ctx;
        (void)dtMs;
    }
    
    CompanionState getStateId() const override { return CompanionState::ERROR_STATE; }
};

// --- Static State Instances ---

static BootState bootState;
static IdleState idleState;
static SleepingState sleepingState;
static ObservingState observingState;
static HappyState happyState;
static ThinkingState thinkingState;
static AlertState alertState;
static ErrorState errorState;

// --- StateMachine Class Implementation ---

StateMachine::StateMachine(EventQueue& queue)
    : currentState(nullptr), eventQueue(queue) {}

void StateMachine::begin() {
    currentState = &bootState;
    currentState->onEnter(*this);
}

void StateMachine::processEvent(const Event& event) {
    if (currentState) {
        currentState->handleEvent(*this, event);
    }
}

void StateMachine::tick(uint32_t dtMs) {
    if (currentState) {
        currentState->tick(*this, dtMs);
    }
}

void StateMachine::transitionTo(CompanionState nextState) {
    if (currentState) {
        currentState->onExit(*this);
    }
    
    switch (nextState) {
        case CompanionState::BOOT:
            currentState = &bootState;
            break;
        case CompanionState::IDLE:
            currentState = &idleState;
            break;
        case CompanionState::SLEEPING:
            currentState = &sleepingState;
            break;
        case CompanionState::OBSERVING:
            currentState = &observingState;
            break;
        case CompanionState::HAPPY:
            currentState = &happyState;
            break;
        case CompanionState::THINKING:
            currentState = &thinkingState;
            break;
        case CompanionState::ALERT:
            currentState = &alertState;
            break;
        case CompanionState::ERROR_STATE:
            currentState = &errorState;
            break;
    }
    
    if (currentState) {
        currentState->onEnter(*this);
        
        // Broadcast change event to external pipeline
        Event ev;
        ev.type = EventType::STATE_CHANGED;
        ev.data.metadata = static_cast<uint16_t>(nextState);
        eventQueue.enqueue(ev);
    }
}

CompanionState StateMachine::getCurrentState() const {
    if (currentState) {
        return currentState->getStateId();
    }
    return CompanionState::BOOT;
}
