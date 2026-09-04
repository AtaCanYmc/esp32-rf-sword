/**
 * @file AttackCoordinator.h
 * @brief Master Attack Coordinator & Execution Engine for ESP32-RF-SWORD
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#pragma once

#include "Types.h"
#include "attack/Presets.h"

class AttackCoordinator {
public:
    static AttackCoordinator& instance();

    void init();
    void setMode(AttackMode mode);
    void applyPreset(TargetPreset preset);

    void step();
    void stop();

    AttackMode getCurrentMode() const;
    TargetPreset getCurrentPreset() const;

private:
    AttackCoordinator();
    ~AttackCoordinator() = default;

    AttackMode currentMode = AttackMode::IDLE;
    TargetPreset currentPreset = TargetPreset::FULL_BAND_2_4GHZ;
    bool modeInitialized = false;

    void executeTotalBlitz();
};
