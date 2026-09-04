/**
 * @file SPIManager.cpp
 * @brief High-Speed SPI Bus Multiplexer implementation
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#include "hal/SPIManager.h"
#include "core/Logger.h"

SPIManager& SPIManager::instance() {
    static SPIManager s_instance;
    return s_instance;
}

SPIManager::SPIManager() {
#ifndef HOST_SIMULATION
    spiMutex = xSemaphoreCreateMutex();
#endif
}

void SPIManager::preConditionBus(const PinConfig& pins) {
#ifndef HOST_SIMULATION
    // Drive all nRF24 CSN pins HIGH and CE pins LOW to keep bus unasserted
    for (int i = 0; i < 4; i++) {
        if (pins.nrf_csn[i] >= 0) {
            pinMode(pins.nrf_csn[i], OUTPUT);
            digitalWrite(pins.nrf_csn[i], HIGH);
        }
        if (pins.nrf_ce[i] >= 0) {
            pinMode(pins.nrf_ce[i], OUTPUT);
            digitalWrite(pins.nrf_ce[i], LOW);
        }
    }

    // Drive Sub-GHz CC1101 CSN HIGH
    if (pins.cc1101_csn >= 0) {
        pinMode(pins.cc1101_csn, OUTPUT);
        digitalWrite(pins.cc1101_csn, HIGH);
    }

    SWORD_LOGD("SPI", "All radio CSN lines pulled HIGH (inactive) for bus isolation.");
#endif
}

bool SPIManager::init(const PinConfig& pins, uint32_t speedHz) {
    spiSpeed = speedHz;
    preConditionBus(pins);

#ifndef HOST_SIMULATION
    if (spiBus == nullptr) {
#if defined(CONFIG_IDF_TARGET_ESP32C3) || defined(BOARD_ESP32_C3_SUPERMINI) || defined(CONFIG_IDF_TARGET_ESP32S3) || defined(BOARD_ESP32_S3_DEVKIT) || defined(CONFIG_IDF_TARGET_ESP32C6)
        spiBus = new SPIClass(FSPI);
#else
        spiBus = new SPIClass(VSPI);
#endif
    }

    spiBus->begin(pins.sck, pins.miso, pins.mosi, -1);
    initialized = true;
    SWORD_LOGS("SPI", "SPI bus initialized @ %u Hz (SCK=%d, MISO=%d, MOSI=%d)", spiSpeed, pins.sck, pins.miso, pins.mosi);
    return true;
#else
    initialized = true;
    return true;
#endif
}

#ifndef HOST_SIMULATION
SPIClass* SPIManager::getSPI() {
    return spiBus;
}

bool SPIManager::lockBus(TickType_t timeout) {
    if (spiMutex == nullptr) return true;
    return (xSemaphoreTake(spiMutex, timeout) == pdTRUE);
}

void SPIManager::unlockBus() {
    if (spiMutex != nullptr) {
        xSemaphoreGive(spiMutex);
    }
}
#endif

uint32_t SPIManager::getClockSpeed() const {
    return spiSpeed;
}

void SPIManager::setClockSpeed(uint32_t speedHz) {
    spiSpeed = speedHz;
}
