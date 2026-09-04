/**
 * @file SweepAttack.h
 * @brief Advanced Coprime Multi-Radio 2.4GHz Sweeper Engine
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#pragma once

#include "Types.h"
#include <stdint.h>

class SweepAttack {
public:
    static SweepAttack& instance();

    void init();
    void stepCoprimeSweep();
    void stepLinearSweep();
    void stepRandomSweep();

    void reset();

private:
    SweepAttack();
    ~SweepAttack() = default;

    uint8_t sweepIndex = 0;
    bool linearDirectionUp = true;
    uint8_t linearCurrentCh = 2;
};
