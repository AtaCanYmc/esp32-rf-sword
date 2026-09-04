/**
 * @file Presets.cpp
 * @brief Pre-configured frequency targets implementation
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#include "attack/Presets.h"
#include <string.h>

namespace {

const TargetProfile PROFILES[] = {
    // 0: Full Band 2.4GHz
    {
        TargetPreset::FULL_BAND_2_4GHZ,
        "full",
        "2.4 GHz Full ISM Band (2402 - 2480 MHz)",
        2, 80,
        {}, 0,
        120, 180,
        0.0f
    },
    // 1: BLE Primary Advertisement Channels (37, 38, 39)
    {
        TargetPreset::BLE_ADVERTISEMENT,
        "ble-adv",
        "Bluetooth Low Energy Primary Adv (2402, 2426, 2480 MHz)",
        2, 80,
        {2, 26, 80}, 3, // Channel 2 = 2402MHz (37), Channel 26 = 2426MHz (38), Channel 80 = 2480MHz (39)
        80, 140,
        0.0f
    },
    // 2: BLE Full Band
    {
        TargetPreset::BLE_ALL_CHANNELS,
        "ble-all",
        "Bluetooth Low Energy All 40 Channels (2402 - 2480 MHz)",
        2, 80,
        {}, 0,
        100, 150,
        0.0f
    },
    // 3: Wi-Fi Channel 1
    {
        TargetPreset::WIFI_CHANNEL_1,
        "wifi-1",
        "Wi-Fi Channel 1 Center 2412 MHz (2401 - 2423 MHz)",
        1, 23,
        {}, 0,
        90, 140,
        0.0f
    },
    // 4: Wi-Fi Channel 6
    {
        TargetPreset::WIFI_CHANNEL_6,
        "wifi-6",
        "Wi-Fi Channel 6 Center 2437 MHz (2426 - 2448 MHz)",
        26, 48,
        {}, 0,
        90, 140,
        0.0f
    },
    // 5: Wi-Fi Channel 11
    {
        TargetPreset::WIFI_CHANNEL_11,
        "wifi-11",
        "Wi-Fi Channel 11 Center 2462 MHz (2451 - 2473 MHz)",
        51, 73,
        {}, 0,
        90, 140,
        0.0f
    },
    // 6: Wi-Fi All Channels
    {
        TargetPreset::WIFI_ALL_CHANNELS,
        "wifi-all",
        "Wi-Fi All Channels 1 - 13 (2401 - 2473 MHz)",
        1, 73,
        {}, 0,
        100, 160,
        0.0f
    },
    // 7: ZigBee All Channels (11 to 26)
    {
        TargetPreset::ZIGBEE_ALL,
        "zigbee",
        "ZigBee / Thread 802.15.4 (2405 - 2480 MHz, 5MHz Spacing)",
        5, 80,
        {5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80}, 16,
        100, 150,
        0.0f
    },
    // 8: RC Drone FlySky AFHDS
    {
        TargetPreset::RC_DRONE_FLYSKY,
        "flysky",
        "FlySky AFHDS / AFHDS 2A 2.4GHz Spread Spectrum",
        5, 80,
        {}, 0,
        80, 120,
        0.0f
    },
    // 9: RC Drone FrSky ACCST
    {
        TargetPreset::RC_DRONE_FRSKY,
        "frsky",
        "FrSky ACCST / ACCESS 2.4GHz RC Link",
        4, 78,
        {}, 0,
        80, 120,
        0.0f
    },
    // 10: RC ExpressLRS 2.4GHz
    {
        TargetPreset::RC_DRONE_EXPRESSLRS,
        "elrs",
        "ExpressLRS 2.4GHz High Rate Link",
        2, 80,
        {}, 0,
        60, 100,
        0.0f
    },
    // 11: Sub-GHz 315 MHz
    {
        TargetPreset::SUBGHZ_315MHZ,
        "sub-315",
        "Sub-GHz 315.00 MHz Gate / Car Remote ISM",
        0, 0,
        {}, 0,
        1000, 2000,
        315.00f
    },
    // 12: Sub-GHz 433 MHz
    {
        TargetPreset::SUBGHZ_433MHZ,
        "sub-433",
        "Sub-GHz 433.92 MHz Car Keys / Alarms / Weather",
        0, 0,
        {}, 0,
        1000, 2000,
        433.92f
    },
    // 13: Sub-GHz 868 MHz
    {
        TargetPreset::SUBGHZ_868MHZ,
        "sub-868",
        "Sub-GHz 868.35 MHz European ISM / LoRa / Smart Meter",
        0, 0,
        {}, 0,
        1000, 2000,
        868.35f
    },
    // 14: Sub-GHz 915 MHz
    {
        TargetPreset::SUBGHZ_915MHZ,
        "sub-915",
        "Sub-GHz 915.00 MHz US ISM / LoRa / Drone Telemetry",
        0, 0,
        {}, 0,
        1000, 2000,
        915.00f
    },
    // 15: Custom Range
    {
        TargetPreset::CUSTOM_RANGE,
        "custom",
        "User Defined Custom Channel & Dwell Range",
        2, 80,
        {}, 0,
        120, 180,
        433.92f
    }
};

constexpr size_t PROFILE_COUNT = sizeof(PROFILES) / sizeof(PROFILES[0]);

} // namespace

const TargetProfile& Presets::getProfile(TargetPreset preset) {
    for (size_t i = 0; i < PROFILE_COUNT; i++) {
        if (PROFILES[i].preset == preset) {
            return PROFILES[i];
        }
    }
    return PROFILES[0];
}

TargetPreset Presets::getPresetByName(const char* name) {
    if (name == nullptr) return TargetPreset::FULL_BAND_2_4GHZ;
    for (size_t i = 0; i < PROFILE_COUNT; i++) {
        if (strcasecmp(PROFILES[i].name, name) == 0) {
            return PROFILES[i].preset;
        }
    }
    return TargetPreset::FULL_BAND_2_4GHZ;
}

size_t Presets::getProfileCount() {
    return PROFILE_COUNT;
}

const TargetProfile& Presets::getProfileByIndex(size_t index) {
    if (index >= PROFILE_COUNT) return PROFILES[0];
    return PROFILES[index];
}
