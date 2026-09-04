/**
 * @file CC1101Controller.h
 * @brief Sub-GHz Transceiver Controller for TI CC1101 (315 / 433 / 868 / 915 MHz)
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#pragma once

#include "Types.h"
#include "hal/PinDefinitions.h"

#ifndef HOST_SIMULATION
#include <RadioLib.h>
#endif

class CC1101Controller {
public:
    static CC1101Controller& instance();

    bool init(const PinConfig& pins, float defaultFreq = 433.92f);
    bool isConnected() const;

    bool setFrequency(float freqMHz);
    bool setModulation(SubGhzModulation mod);
    bool setOutputPower(int8_t powerDbm);

    void startTransmitCW(float freqMHz);
    void startNoiseJammer(float freqMHz);
    void sweepBand(float startFreqMHz, float endFreqMHz, float stepMHz, uint16_t dwellMs);
    void stop();

    float getFrequency() const;
    int8_t getRssi();

private:
    CC1101Controller();
    ~CC1101Controller();

    bool isReady = false;
    float currentFrequency = 433.92f;
    SubGhzModulation currentMod = SubGhzModulation::MOD_CW;
    int8_t currentPower = 10;
    PinConfig activePins;

#ifndef HOST_SIMULATION
    CC1101* radio = nullptr;
    Module* module = nullptr;
#endif
};
