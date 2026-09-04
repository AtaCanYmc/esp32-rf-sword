/**
 * @file SystemState.cpp
 * @brief Central atomic runtime state implementation
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#include "core/SystemState.h"
#include <string.h>

#ifndef HOST_SIMULATION
#include <Arduino.h>
#include <esp_system.h>
#if defined(ESP32) && !defined(CONFIG_IDF_TARGET_ESP32C3)
#include <esp_system.h>
#endif
#else
#include <chrono>
#endif

SystemState& SystemState::instance() {
    static SystemState s_instance;
    return s_instance;
}

SystemState::SystemState() {
#ifndef HOST_SIMULATION
    stateMutex = xSemaphoreCreateMutex();
#endif
    memset(&telemetry, 0, sizeof(telemetry));
    telemetry.minChannel = config.minChannel;
    telemetry.maxChannel = config.maxChannel;
    telemetry.minDwellUs = config.minDwellUs;
    telemetry.maxDwellUs = config.maxDwellUs;
    telemetry.powerLevel = config.powerLevel;
    telemetry.activeMode = config.defaultMode;
    telemetry.activePreset = config.defaultPreset;
    isRunningFlag = config.autoStartOnBoot;
}

void SystemState::init() {
    updateSystemMetrics();
}

void SystemState::setMode(AttackMode mode) {
#ifndef HOST_SIMULATION
    if (stateMutex) xSemaphoreTake(stateMutex, portMAX_DELAY);
#endif
    config.defaultMode = mode;
    telemetry.activeMode = mode;
#ifndef HOST_SIMULATION
    if (stateMutex) xSemaphoreGive(stateMutex);
#endif
}

AttackMode SystemState::getMode() const {
    return telemetry.activeMode;
}

void SystemState::setPreset(TargetPreset preset) {
#ifndef HOST_SIMULATION
    if (stateMutex) xSemaphoreTake(stateMutex, portMAX_DELAY);
#endif
    config.defaultPreset = preset;
    telemetry.activePreset = preset;
#ifndef HOST_SIMULATION
    if (stateMutex) xSemaphoreGive(stateMutex);
#endif
}

TargetPreset SystemState::getPreset() const {
    return telemetry.activePreset;
}

void SystemState::setPowerLevel(RadioPowerLevel level) {
#ifndef HOST_SIMULATION
    if (stateMutex) xSemaphoreTake(stateMutex, portMAX_DELAY);
#endif
    config.powerLevel = level;
    telemetry.powerLevel = level;
#ifndef HOST_SIMULATION
    if (stateMutex) xSemaphoreGive(stateMutex);
#endif
}

RadioPowerLevel SystemState::getPowerLevel() const {
    return telemetry.powerLevel;
}

void SystemState::setChannels(uint8_t minCh, uint8_t maxCh) {
    if (minCh > maxCh) {
        uint8_t tmp = minCh;
        minCh = maxCh;
        maxCh = tmp;
    }
    if (maxCh > NRF24_MAX_CHANNEL) maxCh = NRF24_MAX_CHANNEL;

#ifndef HOST_SIMULATION
    if (stateMutex) xSemaphoreTake(stateMutex, portMAX_DELAY);
#endif
    config.minChannel = minCh;
    config.maxChannel = maxCh;
    telemetry.minChannel = minCh;
    telemetry.maxChannel = maxCh;
#ifndef HOST_SIMULATION
    if (stateMutex) xSemaphoreGive(stateMutex);
#endif
}

void SystemState::getChannels(uint8_t& minCh, uint8_t& maxCh) const {
    minCh = telemetry.minChannel;
    maxCh = telemetry.maxChannel;
}

void SystemState::setDwellRangeUs(uint16_t minDwell, uint16_t maxDwell) {
    if (minDwell < ABSOLUTE_MIN_DWELL_US) minDwell = ABSOLUTE_MIN_DWELL_US;
    if (maxDwell > ABSOLUTE_MAX_DWELL_US) maxDwell = ABSOLUTE_MAX_DWELL_US;
    if (minDwell > maxDwell) minDwell = maxDwell;

#ifndef HOST_SIMULATION
    if (stateMutex) xSemaphoreTake(stateMutex, portMAX_DELAY);
#endif
    config.minDwellUs = minDwell;
    config.maxDwellUs = maxDwell;
    telemetry.minDwellUs = minDwell;
    telemetry.maxDwellUs = maxDwell;
#ifndef HOST_SIMULATION
    if (stateMutex) xSemaphoreGive(stateMutex);
#endif
}

void SystemState::getDwellRangeUs(uint16_t& minDwell, uint16_t& maxDwell) const {
    minDwell = telemetry.minDwellUs;
    maxDwell = telemetry.maxDwellUs;
}

void SystemState::setSubGhzParams(float freqMHz, SubGhzModulation mod, int8_t powerDbm) {
#ifndef HOST_SIMULATION
    if (stateMutex) xSemaphoreTake(stateMutex, portMAX_DELAY);
#endif
    config.subGhzFrequencyMHz = freqMHz;
    config.subGhzMod = mod;
    config.subGhzPowerDbm = powerDbm;
#ifndef HOST_SIMULATION
    if (stateMutex) xSemaphoreGive(stateMutex);
#endif
}

void SystemState::getSubGhzParams(float& freqMHz, SubGhzModulation& mod, int8_t& powerDbm) const {
    freqMHz = config.subGhzFrequencyMHz;
    mod = config.subGhzMod;
    powerDbm = config.subGhzPowerDbm;
}

void SystemState::recordHops(uint8_t radioIndex, uint32_t count) {
    if (radioIndex < MAX_NRF24_MODULES) {
        radioStatuses[radioIndex].channelHops += count;
    }
    telemetry.totalHopsAllTime += count;
}

void SystemState::recordPackets(uint8_t radioIndex, uint32_t count) {
    if (radioIndex < MAX_NRF24_MODULES) {
        radioStatuses[radioIndex].packetsSent += count;
    }
    telemetry.totalPacketsAllTime += count;
}

void SystemState::updateRadioStatus(uint8_t radioIndex, bool active, uint8_t channel, float freqMHz, int8_t rssi) {
    if (radioIndex < MAX_NRF24_MODULES) {
        radioStatuses[radioIndex].active = active;
        radioStatuses[radioIndex].currentChannel = channel;
        radioStatuses[radioIndex].currentFrequencyMHz = freqMHz;
        radioStatuses[radioIndex].lastRssi = rssi;
    }
}

void SystemState::setRadioPresent(uint8_t radioIndex, bool present) {
    if (radioIndex < MAX_NRF24_MODULES) {
        radioStatuses[radioIndex].present = present;
    }
}

TelemetryData SystemState::getTelemetry() {
    updateSystemMetrics();
    return telemetry;
}

RadioStatus SystemState::getRadioStatus(uint8_t radioIndex) const {
    if (radioIndex < MAX_NRF24_MODULES) {
        return radioStatuses[radioIndex];
    }
    return RadioStatus();
}

uint8_t SystemState::getActiveRadioCount() const {
    uint8_t count = 0;
    for (size_t i = 0; i < MAX_NRF24_MODULES; i++) {
        if (radioStatuses[i].present) count++;
    }
    return count;
}

void SystemState::updateSystemMetrics() {
#ifndef HOST_SIMULATION
    uint32_t now = millis();
    telemetry.uptimeSeconds = now / 1000;
    telemetry.freeHeapBytes = esp_get_free_heap_size();
    telemetry.minFreeHeapBytes = esp_get_minimum_free_heap_size();
    telemetry.activeRadioCount = getActiveRadioCount();

    // Calculate rates per second
    uint32_t deltaMs = now - lastMetricUpdateTime;
    if (deltaMs >= 1000) {
        uint32_t currentHops = telemetry.totalHopsAllTime;
        uint32_t currentPkts = telemetry.totalPacketsAllTime;
        
        telemetry.totalHopsPerSecond = (currentHops - lastHopSnapshot) * 1000 / deltaMs;
        telemetry.totalPacketsPerSecond = (currentPkts - lastPktSnapshot) * 1000 / deltaMs;
        
        lastHopSnapshot = currentHops;
        lastPktSnapshot = currentPkts;
        lastMetricUpdateTime = now;
    }
#else
    telemetry.uptimeSeconds += 1;
    telemetry.freeHeapBytes = 256000;
    telemetry.minFreeHeapBytes = 200000;
#endif
}

void SystemState::setBoardType(BoardType board) {
    detectedBoard = board;
}

BoardType SystemState::getBoardType() const {
    return detectedBoard;
}

void SystemState::setConfig(const DeviceConfig& cfg) {
#ifndef HOST_SIMULATION
    if (stateMutex) xSemaphoreTake(stateMutex, portMAX_DELAY);
#endif
    config = cfg;
#ifndef HOST_SIMULATION
    if (stateMutex) xSemaphoreGive(stateMutex);
#endif
}

DeviceConfig SystemState::getConfig() const {
    return config;
}

bool SystemState::isRunning() const {
    return isRunningFlag;
}

void SystemState::start() {
    isRunningFlag = true;
}

void SystemState::stop() {
    isRunningFlag = false;
    for (size_t i = 0; i < MAX_NRF24_MODULES; i++) {
        radioStatuses[i].active = false;
    }
}

const char* SystemState::getModeName(AttackMode mode) const {
    switch (mode) {
        case AttackMode::IDLE:               return "IDLE";
        case AttackMode::SWEEP_COPRIME:      return "SWEEP_COPRIME";
        case AttackMode::SWEEP_LINEAR:       return "SWEEP_LINEAR";
        case AttackMode::SWEEP_RANDOM:       return "SWEEP_RANDOM";
        case AttackMode::TARGETED_BLE:       return "TARGETED_BLE";
        case AttackMode::TARGETED_WIFI:      return "TARGETED_WIFI";
        case AttackMode::TARGETED_ZIGBEE:    return "TARGETED_ZIGBEE";
        case AttackMode::TARGETED_DRONE:     return "TARGETED_DRONE";
        case AttackMode::TARGETED_SUBGHZ:    return "TARGETED_SUBGHZ";
        case AttackMode::NOISE_BURST:        return "NOISE_BURST";
        case AttackMode::SPECTRUM_ANALYZER:  return "SPECTRUM_ANALYZER";
        case AttackMode::WIFI_BEACON_SPAM:   return "WIFI_BEACON_SPAM";
        case AttackMode::WIFI_DEAUTH_STORM:  return "WIFI_DEAUTH_STORM";
        case AttackMode::BLE_POPUP_SPAM:     return "BLE_POPUP_SPAM";
        case AttackMode::TOTAL_BLITZ:        return "TOTAL_BLITZ";
        default:                             return "UNKNOWN";
    }
}

const char* SystemState::getPresetName(TargetPreset preset) const {
    switch (preset) {
        case TargetPreset::FULL_BAND_2_4GHZ:     return "Full Band 2.4GHz (2402-2480 MHz)";
        case TargetPreset::BLE_ADVERTISEMENT:    return "BLE Adv (Ch 37, 38, 39)";
        case TargetPreset::BLE_ALL_CHANNELS:     return "BLE Full Band (37 Data + 3 Adv)";
        case TargetPreset::WIFI_CHANNEL_1:       return "Wi-Fi Ch 1 (2401-2423 MHz)";
        case TargetPreset::WIFI_CHANNEL_6:       return "Wi-Fi Ch 6 (2426-2448 MHz)";
        case TargetPreset::WIFI_CHANNEL_11:      return "Wi-Fi Ch 11 (2451-2473 MHz)";
        case TargetPreset::WIFI_ALL_CHANNELS:    return "Wi-Fi Full Band (Ch 1-14)";
        case TargetPreset::ZIGBEE_ALL:           return "ZigBee 802.15.4 (Ch 11-26)";
        case TargetPreset::RC_DRONE_FLYSKY:      return "RC FlySky AFHDS Hopping";
        case TargetPreset::RC_DRONE_FRSKY:       return "RC FrSky ACCST Hopping";
        case TargetPreset::RC_DRONE_EXPRESSLRS:  return "RC ExpressLRS 2.4GHz";
        case TargetPreset::SUBGHZ_315MHZ:        return "Sub-GHz 315.00 MHz";
        case TargetPreset::SUBGHZ_433MHZ:        return "Sub-GHz 433.92 MHz";
        case TargetPreset::SUBGHZ_868MHZ:        return "Sub-GHz 868.35 MHz";
        case TargetPreset::SUBGHZ_915MHZ:        return "Sub-GHz 915.00 MHz";
        case TargetPreset::CUSTOM_RANGE:         return "Custom Range";
        default:                                 return "Unknown Preset";
    }
}

const char* SystemState::getPowerLevelName(RadioPowerLevel power) const {
    switch (power) {
        case RadioPowerLevel::POWER_MIN:  return "MIN (-18 dBm)";
        case RadioPowerLevel::POWER_LOW:  return "LOW (-12 dBm)";
        case RadioPowerLevel::POWER_HIGH: return "HIGH (-6 dBm)";
        case RadioPowerLevel::POWER_MAX:  return "MAX (0 dBm / +20dBm PA)";
        default:                          return "UNKNOWN";
    }
}

const char* SystemState::getBoardTypeName(BoardType board) const {
    switch (board) {
        case BoardType::ESP32_C3_SUPERMINI: return "ESP32-C3 SuperMini";
        case BoardType::ESP32_DEVKIT_V1:    return "ESP32 DevKit V1 (WROOM)";
        case BoardType::ESP32_S3_DEVKIT:    return "ESP32-S3 DevKit";
        case BoardType::ESP32_C6_DEVKIT:    return "ESP32-C6 DevKit";
        case BoardType::CUSTOM_PINOUT:      return "Custom Pinout";
        default:                            return "Auto-Detect";
    }
}
