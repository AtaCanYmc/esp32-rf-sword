/**
 * @file TargetedAttack.h
 * @brief Targeted Protocol Frequency Jammer Engine
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#pragma once

#include "Types.h"
#include "attack/Presets.h"
#include <stdint.h>

class TargetedAttack {
public:
    static TargetedAttack& instance();

    void init(TargetPreset preset);
    void stepTargetedJammer();

private:
    TargetedAttack();
    ~TargetedAttack() = default;

    TargetPreset activePreset = TargetPreset::BLE_ADVERTISEMENT;
    uint8_t targetChannels[32];
    uint8_t targetChannelCount = 0;
    uint8_t currentTargetIndex = 0;
};
