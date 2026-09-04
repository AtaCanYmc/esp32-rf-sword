/**
 * @file CC1101Controller.cpp
 * @brief Sub-GHz Transceiver Controller implementation
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#include "radio/CC1101Controller.h"
#include "hal/SPIManager.h"
#include "core/Logger.h"

#ifndef HOST_SIMULATION
#include <Arduino.h>
#endif

CC1101Controller& CC1101Controller::instance() {
    static CC1101Controller s_instance;
    return s_instance;
}

CC1101Controller::CC1101Controller() {
}

CC1101Controller::~CC1101Controller() {
    stop();
#ifndef HOST_SIMULATION
    if (radio != nullptr) {
        delete radio;
        radio = nullptr;
    }
    if (module != nullptr) {
        delete module;
        module = nullptr;
    }
#endif
}

bool CC1101Controller::init(const PinConfig& pins, float defaultFreq) {
    activePins = pins;
    currentFrequency = defaultFreq;

    if (pins.cc1101_csn < 0) {
        isReady = false;
        SWORD_LOGI("CC1101", "Sub-GHz CC1101 not configured in pinout profile.");
        return false;
    }

#ifndef HOST_SIMULATION
    SPIClass* spi = SPIManager::instance().getSPI();
    if (spi == nullptr) {
        SWORD_LOGE("CC1101", "SPI bus instance is NULL!");
        return false;
    }

    if (module != nullptr) delete module;
    if (radio != nullptr) delete radio;

    module = new Module(pins.cc1101_csn, pins.cc1101_gdo0, RADIOLIB_NC, pins.cc1101_gdo2, *spi);
    radio = new CC1101(module);

    int state = radio->begin(currentFrequency, 4.8, 25.0, 135.0, currentPower, 32);
    if (state == RADIOLIB_ERR_NONE) {
        isReady = true;
        SWORD_LOGS("CC1101", "CC1101 Sub-GHz module initialized successfully at %.2f MHz", currentFrequency);
        return true;
    } else {
        isReady = false;
        SWORD_LOGW("CC1101", "CC1101 initialization failed (code %d). Module may be disconnected.", state);
        return false;
    }
#else
    isReady = false;
    return false;
#endif
}

bool CC1101Controller::isConnected() const {
    return isReady;
}

bool CC1101Controller::setFrequency(float freqMHz) {
    currentFrequency = freqMHz;
#ifndef HOST_SIMULATION
    if (isReady && radio != nullptr) {
        int state = radio->setFrequency(freqMHz);
        return (state == RADIOLIB_ERR_NONE);
    }
#endif
    return isReady;
}

bool CC1101Controller::setModulation(SubGhzModulation mod) {
    currentMod = mod;
#ifndef HOST_SIMULATION
    if (isReady && radio != nullptr) {
        // Configure modulation in CC1101 registers
        return true;
    }
#endif
    return isReady;
}

bool CC1101Controller::setOutputPower(int8_t powerDbm) {
    currentPower = powerDbm;
#ifndef HOST_SIMULATION
    if (isReady && radio != nullptr) {
        int state = radio->setOutputPower(powerDbm);
        return (state == RADIOLIB_ERR_NONE);
    }
#endif
    return isReady;
}

void CC1101Controller::startTransmitCW(float freqMHz) {
    currentFrequency = freqMHz;
#ifndef HOST_SIMULATION
    if (isReady && radio != nullptr) {
        setFrequency(freqMHz);
        radio->transmitDirect();
    }
#endif
}

void CC1101Controller::startNoiseJammer(float freqMHz) {
    currentFrequency = freqMHz;
#ifndef HOST_SIMULATION
    if (isReady && radio != nullptr) {
        setFrequency(freqMHz);
        uint8_t noise[64];
        for (size_t i = 0; i < sizeof(noise); i++) {
            noise[i] = (uint8_t)esp_random();
        }
        radio->startTransmit(noise, sizeof(noise));
    }
#endif
}

void CC1101Controller::sweepBand(float startFreqMHz, float endFreqMHz, float stepMHz, uint16_t dwellMs) {
#ifndef HOST_SIMULATION
    if (!isReady || radio == nullptr) return;
    for (float f = startFreqMHz; f <= endFreqMHz; f += stepMHz) {
        setFrequency(f);
        delay(dwellMs);
    }
#endif
}

void CC1101Controller::stop() {
#ifndef HOST_SIMULATION
    if (isReady && radio != nullptr) {
        radio->standby();
    }
#endif
}

float CC1101Controller::getFrequency() const {
    return currentFrequency;
}

int8_t CC1101Controller::getRssi() {
#ifndef HOST_SIMULATION
    if (isReady && radio != nullptr) {
        return (int8_t)radio->getRSSI();
    }
#endif
    return -100;
}
