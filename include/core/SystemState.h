/**
 * @file SystemState.h
 * @brief Central atomic runtime state and telemetry manager for ESP32-RF-SWORD
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#pragma once

#include "Types.h"
#include "Config.h"

#ifndef HOST_SIMULATION
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#endif

class SystemState {
public:
    static SystemState& instance();

    void init();
    
    // Mode & Settings Controls
    void setMode(AttackMode mode);
    AttackMode getMode() const;
    
    void setPreset(TargetPreset preset);
    TargetPreset getPreset() const;
    
    void setPowerLevel(RadioPowerLevel level);
    RadioPowerLevel getPowerLevel() const;
    
    void setChannels(uint8_t minCh, uint8_t maxCh);
    void getChannels(uint8_t& minCh, uint8_t& maxCh) const;
    
    void setDwellRangeUs(uint16_t minDwell, uint16_t maxDwell);
    void getDwellRangeUs(uint16_t& minDwell, uint16_t& maxDwell) const;

    void setSubGhzParams(float freqMHz, SubGhzModulation mod, int8_t powerDbm);
    void getSubGhzParams(float& freqMHz, SubGhzModulation& mod, int8_t& powerDbm) const;

    // Telemetry & Metrics
    void recordHops(uint8_t radioIndex, uint32_t count = 1);
    void recordPackets(uint8_t radioIndex, uint32_t count = 1);
    void updateRadioStatus(uint8_t radioIndex, bool active, uint8_t channel, float freqMHz, int8_t rssi = -100);
    void setRadioPresent(uint8_t radioIndex, bool present);
    
    TelemetryData getTelemetry();
    RadioStatus getRadioStatus(uint8_t radioIndex) const;
    uint8_t getActiveRadioCount() const;

    void updateSystemMetrics();
    
    // Board model & configuration
    void setBoardType(BoardType board);
    BoardType getBoardType() const;
    
    void setConfig(const DeviceConfig& cfg);
    DeviceConfig getConfig() const;

    bool isRunning() const;
    void start();
    void stop();

    // Helper serialization
    const char* getModeName(AttackMode mode) const;
    const char* getPresetName(TargetPreset preset) const;
    const char* getPowerLevelName(RadioPowerLevel power) const;
    const char* getBoardTypeName(BoardType board) const;

private:
    SystemState();
    ~SystemState() = default;
    SystemState(const SystemState&) = delete;
    SystemState& operator=(const SystemState&) = delete;

    mutable DeviceConfig config;
    mutable TelemetryData telemetry;
    mutable RadioStatus radioStatuses[MAX_NRF24_MODULES];
    BoardType detectedBoard = BoardType::AUTO_DETECT;
    
    bool isRunningFlag = false;
    uint32_t lastMetricUpdateTime = 0;
    uint32_t lastHopSnapshot = 0;
    uint32_t lastPktSnapshot = 0;

#ifndef HOST_SIMULATION
    SemaphoreHandle_t stateMutex = nullptr;
#endif
};
