/**
 * @file NVSManager.h
 * @brief Non-Volatile Storage (Flash Preferences) Manager for ESP32-RF-SWORD
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#pragma once

#include "Types.h"

class NVSManager {
public:
    static bool init();
    static bool loadConfig(DeviceConfig& config);
    static bool saveConfig(const DeviceConfig& config);
    static void resetToDefaults(DeviceConfig& config);

private:
    static const char* NVS_NAMESPACE;
};
