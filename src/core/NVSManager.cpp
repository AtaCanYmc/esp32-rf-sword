/**
 * @file NVSManager.cpp
 * @brief Non-Volatile Storage implementation using ESP32 Preferences
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#include "core/NVSManager.h"
#include "core/Logger.h"
#include "Config.h"
#include <string.h>

#ifndef HOST_SIMULATION
#include <Preferences.h>
static Preferences prefs;
#endif

const char* NVSManager::NVS_NAMESPACE = "rf_sword";

bool NVSManager::init() {
#ifndef HOST_SIMULATION
    return prefs.begin(NVS_NAMESPACE, false);
#else
    return true;
#endif
}

bool NVSManager::loadConfig(DeviceConfig& config) {
#ifndef HOST_SIMULATION
    if (!prefs.begin(NVS_NAMESPACE, true)) {
        SWORD_LOGW("NVS", "Preferences namespace not found, initializing defaults.");
        resetToDefaults(config);
        return false;
    }

    size_t len = prefs.getBytes("cfg", &config, sizeof(DeviceConfig));
    prefs.end();

    if (len != sizeof(DeviceConfig) || config.magic != 0x53575244) {
        SWORD_LOGW("NVS", "Invalid config signature or size mismatch (%zu != %zu). Resetting.", len, sizeof(DeviceConfig));
        resetToDefaults(config);
        saveConfig(config);
        return false;
    }

    SWORD_LOGS("NVS", "Loaded configuration v%u successfully.", config.version);
    return true;
#else
    resetToDefaults(config);
    return true;
#endif
}

bool NVSManager::saveConfig(const DeviceConfig& config) {
#ifndef HOST_SIMULATION
    if (!prefs.begin(NVS_NAMESPACE, false)) {
        SWORD_LOGE("NVS", "Failed to open Preferences for writing.");
        return false;
    }

    size_t written = prefs.putBytes("cfg", &config, sizeof(DeviceConfig));
    prefs.end();

    if (written != sizeof(DeviceConfig)) {
        SWORD_LOGE("NVS", "Failed to write complete config to flash (%zu / %zu bytes).", written, sizeof(DeviceConfig));
        return false;
    }

    SWORD_LOGS("NVS", "Saved configuration successfully.");
    return true;
#else
    return true;
#endif
}

void NVSManager::resetToDefaults(DeviceConfig& config) {
    memset(&config, 0, sizeof(DeviceConfig));
    config.magic = 0x53575244;
    config.version = 2;
    config.autoStartOnBoot = true;
    config.defaultMode = AttackMode::SWEEP_COPRIME;
    config.defaultPreset = TargetPreset::FULL_BAND_2_4GHZ;
    config.powerLevel = RadioPowerLevel::POWER_MAX;
    config.dataRate = RadioDataRate::RATE_2MBPS;
    config.minChannel = 2;
    config.maxChannel = 80;
    config.sweepStep = 37;
    config.radioOffset = 39;
    config.minDwellUs = 120;
    config.maxDwellUs = 180;
    config.spiSpeedHz = 16000000UL;
    
    config.wifiApEnabled = true;
    strncpy(config.apSsid, DEFAULT_AP_SSID, sizeof(config.apSsid) - 1);
    strncpy(config.apPassword, DEFAULT_AP_PASSWORD, sizeof(config.apPassword) - 1);
    config.apChannel = 1;
    
    config.oledEnabled = true;
    config.serialCliEnabled = true;
    
    config.subGhzFrequencyMHz = 433.92f;
    config.subGhzMod = SubGhzModulation::MOD_CW;
    config.subGhzPowerDbm = 10;
    
    // Set invalid pins by default to trigger auto-detection from BoardProfiles
    config.pins.sck = -1;
    config.pins.miso = -1;
    config.pins.mosi = -1;
    for (int i = 0; i < 4; i++) {
        config.pins.nrf_ce[i] = -1;
        config.pins.nrf_csn[i] = -1;
    }
    config.pins.cc1101_csn = -1;
    config.pins.cc1101_gdo0 = -1;
    config.pins.cc1101_gdo2 = -1;
    config.pins.i2c_sda = -1;
    config.pins.i2c_scl = -1;
    config.pins.status_led = -1;
    config.pins.action_button = -1;
}
