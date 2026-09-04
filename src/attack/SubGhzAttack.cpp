/**
 * @file SubGhzAttack.cpp
 * @brief Sub-GHz CC1101 Continuous Wave & Sweep Jammer Coordinator implementation
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#include "attack/SubGhzAttack.h"
#include "radio/CC1101Controller.h"
#include "core/SystemState.h"

#ifndef HOST_SIMULATION
#include <Arduino.h>
#endif

SubGhzAttack& SubGhzAttack::instance() {
    static SubGhzAttack s_instance;
    return s_instance;
}

SubGhzAttack::SubGhzAttack() {
}

void SubGhzAttack::init(float freqMHz, SubGhzModulation mod) {
    activeFrequency = freqMHz;
    activeMod = mod;
    currentSweepFreq = freqMHz - 2.0f;

    if (CC1101Controller::instance().isConnected()) {
        if (mod == SubGhzModulation::MOD_CW) {
            CC1101Controller::instance().startTransmitCW(freqMHz);
        } else {
            CC1101Controller::instance().startNoiseJammer(freqMHz);
        }
    }
}

void SubGhzAttack::stepSubGhz() {
    if (!CC1101Controller::instance().isConnected()) {
#ifndef HOST_SIMULATION
        delay(100);
#endif
        return;
    }

    if (activeMod == SubGhzModulation::MOD_CW) {
        // Continuous wave already transmitted by CC1101 in hardware
#ifndef HOST_SIMULATION
        delay(50);
#endif
    } else {
        // Sweep / Noise modulation
        currentSweepFreq += 0.1f;
        if (currentSweepFreq > (activeFrequency + 2.0f)) {
            currentSweepFreq = activeFrequency - 2.0f;
        }
        CC1101Controller::instance().setFrequency(currentSweepFreq);
#ifndef HOST_SIMULATION
        delay(2);
#endif
    }
}
