/**
 * @file NoiseBurstAttack.h
 * @brief High-Power Corrupted Packet & Noise Frame Blaster Engine
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#pragma once

#include "Types.h"
#include <stdint.h>
#include <stddef.h>

class NoiseBurstAttack {
public:
    static NoiseBurstAttack& instance();

    void init();
    void stepNoiseBurst();

private:
    NoiseBurstAttack();
    ~NoiseBurstAttack() = default;

    uint8_t noiseBuffer[32];
    uint8_t currentChannel = 2;
};
