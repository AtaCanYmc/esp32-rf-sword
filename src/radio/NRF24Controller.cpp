/**
 * @file NRF24Controller.cpp
 * @brief High-Speed Multi-nRF24 Transceiver Controller implementation
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#include "radio/NRF24Controller.h"
#include "hal/SPIManager.h"
#include "core/Logger.h"
#include "core/SystemState.h"

#ifndef HOST_SIMULATION
#include <Arduino.h>
#endif

NRF24Controller& NRF24Controller::instance() {
    static NRF24Controller s_instance;
    return s_instance;
}

NRF24Controller::NRF24Controller() {
    for (int i = 0; i < MAX_NRF24_MODULES; i++) {
#ifndef HOST_SIMULATION
        radios[i] = nullptr;
#endif
        radioReady[i] = false;
    }
}

NRF24Controller::~NRF24Controller() {
    stopAll();
#ifndef HOST_SIMULATION
    for (int i = 0; i < MAX_NRF24_MODULES; i++) {
        if (radios[i] != nullptr) {
            delete radios[i];
            radios[i] = nullptr;
        }
    }
#endif
}

bool NRF24Controller::init(const PinConfig& pins, uint32_t spiSpeed) {
    activePins = pins;
    connectedCount = 0;

    SPIManager::instance().init(pins, spiSpeed);

#ifndef HOST_SIMULATION
    SPIClass* spi = SPIManager::instance().getSPI();
    if (spi == nullptr) {
        SWORD_LOGE("NRF24", "SPI bus instance is NULL!");
        return false;
    }

    // Allocate radio objects
    for (int i = 0; i < MAX_NRF24_MODULES; i++) {
        if (pins.nrf_ce[i] >= 0 && pins.nrf_csn[i] >= 0) {
            if (radios[i] == nullptr) {
                radios[i] = new RF24(pins.nrf_ce[i], pins.nrf_csn[i], spiSpeed);
            }
        }
    }

    // Step 1: Initial begin for all present radios
    for (int i = 0; i < MAX_NRF24_MODULES; i++) {
        if (radios[i] != nullptr) {
            radios[i]->begin(spi);
        }
    }

    // Step 2: Stabilization delay for PA/LNA power rails
    delay(500);

    // Step 3: Second initialization pass & register setup
    for (int i = 0; i < MAX_NRF24_MODULES; i++) {
        if (radios[i] != nullptr) {
            radios[i]->begin(spi);
        }
    }

    configureRadios(currentPower, currentRate);

    // Step 4: Verify chip presence
    for (int i = 0; i < MAX_NRF24_MODULES; i++) {
        if (radios[i] != nullptr) {
            radioReady[i] = radios[i]->isChipConnected();
            SystemState::instance().setRadioPresent(i, radioReady[i]);
            if (radioReady[i]) {
                connectedCount++;
                SWORD_LOGS("NRF24", "Radio #%d detected on CE=%d, CSN=%d", i + 1, pins.nrf_ce[i], pins.nrf_csn[i]);
            } else {
                SWORD_LOGW("NRF24", "Radio #%d FAILED response on CE=%d, CSN=%d", i + 1, pins.nrf_ce[i], pins.nrf_csn[i]);
            }
        } else {
            radioReady[i] = false;
            SystemState::instance().setRadioPresent(i, false);
        }
    }

    SWORD_LOGI("NRF24", "Total connected 2.4GHz radios: %u / %u", connectedCount, MAX_NRF24_MODULES);
    return (connectedCount > 0);
#else
    for (int i = 0; i < 2; i++) {
        radioReady[i] = true;
        SystemState::instance().setRadioPresent(i, true);
        connectedCount++;
    }
    return true;
#endif
}

void NRF24Controller::configureRadios(RadioPowerLevel power, RadioDataRate rate) {
    currentPower = power;
    currentRate = rate;

#ifndef HOST_SIMULATION
    rf24_pa_dbm_e paLevel = RF24_PA_MAX;
    switch (power) {
        case RadioPowerLevel::POWER_MIN:  paLevel = RF24_PA_MIN; break;
        case RadioPowerLevel::POWER_LOW:  paLevel = RF24_PA_LOW; break;
        case RadioPowerLevel::POWER_HIGH: paLevel = RF24_PA_HIGH; break;
        case RadioPowerLevel::POWER_MAX:  paLevel = RF24_PA_MAX; break;
    }

    rf24_datarate_e dataRate = RF24_2MBPS;
    switch (rate) {
        case RadioDataRate::RATE_250KBPS: dataRate = RF24_250KBPS; break;
        case RadioDataRate::RATE_1MBPS:   dataRate = RF24_1MBPS; break;
        case RadioDataRate::RATE_2MBPS:   dataRate = RF24_2MBPS; break;
    }

    for (int i = 0; i < MAX_NRF24_MODULES; i++) {
        if (radios[i] != nullptr) {
            radios[i]->setAutoAck(false);
            radios[i]->stopListening();
            radios[i]->setRetries(0, 0);
            radios[i]->setPayloadSize(32);
            radios[i]->setAddressWidth(5);
            radios[i]->setPALevel(paLevel, true);
            radios[i]->setDataRate(dataRate);
            radios[i]->setCRCLength(RF24_CRC_DISABLED);
        }
    }
#endif
}

bool NRF24Controller::isRadioConnected(uint8_t index) const {
    if (index < MAX_NRF24_MODULES) {
        return radioReady[index];
    }
    return false;
}

uint8_t NRF24Controller::getConnectedCount() const {
    return connectedCount;
}

void NRF24Controller::startConstantCarrierAll(uint8_t startChannelA, uint8_t startChannelB) {
#ifndef HOST_SIMULATION
    rf24_pa_dbm_e paLevel = RF24_PA_MAX;
    switch (currentPower) {
        case RadioPowerLevel::POWER_MIN:  paLevel = RF24_PA_MIN; break;
        case RadioPowerLevel::POWER_LOW:  paLevel = RF24_PA_LOW; break;
        case RadioPowerLevel::POWER_HIGH: paLevel = RF24_PA_HIGH; break;
        case RadioPowerLevel::POWER_MAX:  paLevel = RF24_PA_MAX; break;
    }

    if (radios[0] != nullptr && radioReady[0]) {
        radios[0]->startConstCarrier(paLevel, startChannelA);
    }
    if (radios[1] != nullptr && radioReady[1]) {
        radios[1]->startConstCarrier(paLevel, startChannelB);
    }
    if (radios[2] != nullptr && radioReady[2]) {
        radios[2]->startConstCarrier(paLevel, (startChannelA + 20) % 80);
    }
    if (radios[3] != nullptr && radioReady[3]) {
        radios[3]->startConstCarrier(paLevel, (startChannelB + 20) % 80);
    }
    delay(400); // Settling period before sweeping
#endif
}

void NRF24Controller::startConstantCarrier(uint8_t radioIndex, uint8_t channel) {
#ifndef HOST_SIMULATION
    if (radioIndex < MAX_NRF24_MODULES && radios[radioIndex] != nullptr && radioReady[radioIndex]) {
        rf24_pa_dbm_e paLevel = RF24_PA_MAX;
        switch (currentPower) {
            case RadioPowerLevel::POWER_MIN:  paLevel = RF24_PA_MIN; break;
            case RadioPowerLevel::POWER_LOW:  paLevel = RF24_PA_LOW; break;
            case RadioPowerLevel::POWER_HIGH: paLevel = RF24_PA_HIGH; break;
            case RadioPowerLevel::POWER_MAX:  paLevel = RF24_PA_MAX; break;
        }
        radios[radioIndex]->startConstCarrier(paLevel, channel);
    }
#endif
}

void NRF24Controller::stopAll() {
#ifndef HOST_SIMULATION
    for (int i = 0; i < MAX_NRF24_MODULES; i++) {
        if (radios[i] != nullptr && radioReady[i]) {
            radios[i]->stopConstCarrier();
            radios[i]->powerDown();
        }
    }
#endif
}

void NRF24Controller::setChannel(uint8_t radioIndex, uint8_t channel) {
#ifndef HOST_SIMULATION
    if (radioIndex < MAX_NRF24_MODULES && radios[radioIndex] != nullptr && radioReady[radioIndex]) {
        radios[radioIndex]->setChannel(channel);
    }
#endif
}

void NRF24Controller::sendNoiseBurst(uint8_t radioIndex, const uint8_t* payload, size_t len) {
#ifndef HOST_SIMULATION
    if (radioIndex < MAX_NRF24_MODULES && radios[radioIndex] != nullptr && radioReady[radioIndex]) {
        radios[radioIndex]->writeFast(payload, len, true);
    }
#endif
}

bool NRF24Controller::testCarrier(uint8_t radioIndex) {
#ifndef HOST_SIMULATION
    if (radioIndex < MAX_NRF24_MODULES && radios[radioIndex] != nullptr && radioReady[radioIndex]) {
        return radios[radioIndex]->testCarrier();
    }
    return false;
#else
    return false;
#endif
}

int8_t NRF24Controller::scanRssi(uint8_t radioIndex, uint8_t channel) {
#ifndef HOST_SIMULATION
    if (radioIndex < MAX_NRF24_MODULES && radios[radioIndex] != nullptr && radioReady[radioIndex]) {
        radios[radioIndex]->setChannel(channel);
        radios[radioIndex]->startListening();
        delayMicroseconds(130);
        bool carrier = radios[radioIndex]->testCarrier();
        radios[radioIndex]->stopListening();
        return carrier ? -60 : -100;
    }
    return -100;
#else
    return -100;
#endif
}

void NRF24Controller::setPowerLevel(RadioPowerLevel power) {
    configureRadios(power, currentRate);
}
