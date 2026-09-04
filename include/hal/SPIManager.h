/**
 * @file SPIManager.h
 * @brief High-Speed SPI Bus Multiplexer & Preconditioning Manager
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#pragma once

#include "Types.h"
#include "Config.h"

#ifndef HOST_SIMULATION
#include <SPI.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#endif

class SPIManager {
public:
    static SPIManager& instance();

    bool init(const PinConfig& pins, uint32_t speedHz = DEFAULT_SPI_SPEED_HZ);
    void preConditionBus(const PinConfig& pins);
    
#ifndef HOST_SIMULATION
    SPIClass* getSPI();
    bool lockBus(TickType_t timeout = portMAX_DELAY);
    void unlockBus();
#endif

    uint32_t getClockSpeed() const;
    void setClockSpeed(uint32_t speedHz);

private:
    SPIManager();
    ~SPIManager() = default;

    uint32_t spiSpeed = DEFAULT_SPI_SPEED_HZ;
    bool initialized = false;

#ifndef HOST_SIMULATION
    SPIClass* spiBus = nullptr;
    SemaphoreHandle_t spiMutex = nullptr;
#endif
};
