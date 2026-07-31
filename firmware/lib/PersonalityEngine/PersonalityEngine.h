/**
 * @file PersonalityEngine.h
 * @brief Personality profile structures and scaling constants.
 * 
 * Responsibilities:
 * - Hold fixed configuration profiles (friendliness, excitable scales).
 * - Distribute scaling coefficients to modify the EmotionEngine update cycles.
 * 
 * TODO:
 * - [ ] Implement EEPROM loading and configuration override routines.
 */

#pragma once
#include <stdint.h>

struct PersonalityProfile {
    uint8_t excitability;  // How fast Arousal grows with events [0 - 100]
    uint8_t friendliness;  // How fast Valence grows with touch [0 - 100]
    uint8_t recoveryRate;  // How fast emotions return to baseline [0 - 100]
    uint8_t fearThreshold; // Lower value = more easily scared
};

class PersonalityEngine {
private:
    PersonalityProfile currentProfile;

public:
    PersonalityEngine() {
        // Default personality: Bold but warm
        currentProfile.excitability = 40;
        currentProfile.friendliness = 60;
        currentProfile.recoveryRate = 30;
        currentProfile.fearThreshold = 50;
    }

    /**
     * @brief Retrieve the current active profile.
     */
    const PersonalityProfile& getProfile() const { return currentProfile; }

    /**
     * @brief Dynamically set/override active profile configurations.
     */
    void setProfile(const PersonalityProfile& profile) { currentProfile = profile; }
};
