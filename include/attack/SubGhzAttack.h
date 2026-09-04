/**
 * @file SubGhzAttack.h
 * @brief Sub-GHz CC1101 Continuous Wave & Sweep Jammer Coordinator
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#pragma once

#include "Types.h"
#include <stdint.h>

class SubGhzAttack {
public:
    static SubGhzAttack& instance();

    void init(float freqMHz = 433.92f, SubGhzModulation mod = SubGhzModulation::MOD_CW);
    void stepSubGhz();

private:
    SubGhzAttack();
    ~SubGhzAttack() = default;

    float activeFrequency = 433.92f;
    SubGhzModulation activeMod = SubGhzModulation::MOD_CW;
    bool isSweeping = false;
    float currentSweepFreq = 433.0f;
};
