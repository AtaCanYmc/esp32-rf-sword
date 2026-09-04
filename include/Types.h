/**
 * @file Types.h
 * @brief Core Data Types, Enums and Structs for ESP32-RF-SWORD
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#pragma once

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Hardware board models supported by the firmware
 */
enum class BoardType : uint8_t {
    AUTO_DETECT = 0,
    ESP32_C3_SUPERMINI,
    ESP32_DEVKIT_V1,
    ESP32_S3_DEVKIT,
    ESP32_C6_DEVKIT,
    CUSTOM_PINOUT
};

/**
 * @brief Active operational modes of the RF Sword
 */
enum class AttackMode : uint8_t {
    IDLE = 0,
    SWEEP_COPRIME,       ///< Dual/Multi nRF24 Coprime Carrier Sweeper (Advanced RF-KILL)
    SWEEP_LINEAR,        ///< Fast Linear ping-pong channel sweep
    SWEEP_RANDOM,        ///< Pseudo-random Gaussian jitter sweep
    TARGETED_BLE,        ///< Focused jamming on BLE Adv (37, 38, 39) & Data channels
    TARGETED_WIFI,       ///< Focused jamming on Wi-Fi Ch 1, 6, 11 or custom channel
    TARGETED_ZIGBEE,     ///< Focused jamming on 802.15.4 / ZigBee channels (11-26)
    TARGETED_DRONE,      ///< Focused jamming on RC / Drone ISM hopping bands
    TARGETED_SUBGHZ,     ///< Sub-GHz CW or Sweep jamming (315/433/868/915 MHz) via CC1101
    NOISE_BURST,         ///< High-power corrupted packet / noise frame blasting
    SPECTRUM_ANALYZER,   ///< 2.4 GHz continuous RSSI spectrum sniffer & waterfall
    WIFI_BEACON_SPAM,    ///< ESP32 native 802.11 beacon SSID flood
    WIFI_DEAUTH_STORM,   ///< ESP32 native 802.11 deauth/disassociation frames
    BLE_POPUP_SPAM,      ///< ESP32 native BLE advertisement spam (Apple/Android/Windows)
    TOTAL_BLITZ          ///< Synchronized all-band multi-radio simultaneous disruption
};

/**
 * @brief Pre-configured frequency targets
 */
enum class TargetPreset : uint8_t {
    FULL_BAND_2_4GHZ = 0,
    BLE_ADVERTISEMENT,
    BLE_ALL_CHANNELS,
    WIFI_CHANNEL_1,
    WIFI_CHANNEL_6,
    WIFI_CHANNEL_11,
    WIFI_ALL_CHANNELS,
    ZIGBEE_ALL,
    RC_DRONE_FLYSKY,
    RC_DRONE_FRSKY,
    RC_DRONE_EXPRESSLRS,
    SUBGHZ_315MHZ,
    SUBGHZ_433MHZ,
    SUBGHZ_868MHZ,
    SUBGHZ_915MHZ,
    CUSTOM_RANGE
};

/**
 * @brief Power levels for nRF24L01+ transceivers
 */
enum class RadioPowerLevel : uint8_t {
    POWER_MIN = 0,  // -18 dBm
    POWER_LOW,      // -12 dBm
    POWER_HIGH,     // -6 dBm
    POWER_MAX       // 0 dBm (or +20 dBm with PA/LNA external amplifier)
};

/**
 * @brief Data rates for nRF24L01+ transceivers
 */
enum class RadioDataRate : uint8_t {
    RATE_250KBPS = 0,
    RATE_1MBPS,
    RATE_2MBPS
};

/**
 * @brief Sub-GHz Modulation Schemes for CC1101
 */
enum class SubGhzModulation : uint8_t {
    MOD_CW = 0,     // Continuous Wave unmodulated carrier
    MOD_OOK,        // On-Off Keying pseudo-random pulse
    MOD_2FSK,       // 2-level Frequency Shift Keying noise
    MOD_GFSK        // Gaussian FSK
};

/**
 * @brief Status descriptor for an individual radio hardware instance
 */
struct RadioStatus {
    bool present = false;
    bool active = false;
    uint8_t currentChannel = 0;
    float currentFrequencyMHz = 0.0f;
    uint32_t packetsSent = 0;
    uint32_t channelHops = 0;
    int8_t lastRssi = -100;
};

/**
 * @brief Real-time system telemetry and performance metrics
 */
struct TelemetryData {
    uint32_t uptimeSeconds = 0;
    float coreTemperatureC = 0.0f;
    uint32_t freeHeapBytes = 0;
    uint32_t minFreeHeapBytes = 0;
    uint32_t totalHopsPerSecond = 0;
    uint32_t totalPacketsPerSecond = 0;
    uint32_t totalHopsAllTime = 0;
    uint32_t totalPacketsAllTime = 0;
    AttackMode activeMode = AttackMode::IDLE;
    TargetPreset activePreset = TargetPreset::FULL_BAND_2_4GHZ;
    RadioPowerLevel powerLevel = RadioPowerLevel::POWER_MAX;
    uint8_t activeRadioCount = 0;
    bool wifiApActive = false;
    bool subGhzActive = false;
    uint16_t minDwellUs = 120;
    uint16_t maxDwellUs = 180;
    uint8_t minChannel = 2;
    uint8_t maxChannel = 80;
};

/**
 * @brief Pin configuration for SPI, CSN, CE and I2C buses
 */
struct PinConfig {
    int8_t sck = -1;
    int8_t miso = -1;
    int8_t mosi = -1;
    
    // nRF24 Radios (Up to 4 supported)
    int8_t nrf_ce[4] = {-1, -1, -1, -1};
    int8_t nrf_csn[4] = {-1, -1, -1, -1};
    
    // Sub-GHz CC1101 Radio
    int8_t cc1101_csn = -1;
    int8_t cc1101_gdo0 = -1;
    int8_t cc1101_gdo2 = -1;
    
    // I2C OLED Display
    int8_t i2c_sda = -1;
    int8_t i2c_scl = -1;
    
    // Status LED & Button
    int8_t status_led = -1;
    int8_t action_button = -1;
};

/**
 * @brief Persistent device configuration stored in NVS Flash
 */
struct DeviceConfig {
    uint32_t magic = 0x53575244; // 'SWRD'
    uint16_t version = 2;
    
    bool autoStartOnBoot = true;
    AttackMode defaultMode = AttackMode::SWEEP_COPRIME;
    TargetPreset defaultPreset = TargetPreset::FULL_BAND_2_4GHZ;
    RadioPowerLevel powerLevel = RadioPowerLevel::POWER_MAX;
    RadioDataRate dataRate = RadioDataRate::RATE_2MBPS;
    
    uint8_t minChannel = 2;
    uint8_t maxChannel = 80;
    uint8_t sweepStep = 37;
    uint8_t radioOffset = 39;
    
    uint16_t minDwellUs = 120;
    uint16_t maxDwellUs = 180;
    
    uint32_t spiSpeedHz = 16000000UL;
    
    bool wifiApEnabled = true;
    char apSsid[32] = "SWORD-SECURITY-PORTAL";
    char apPassword[32] = "rfsword123";
    uint8_t apChannel = 1;
    
    bool oledEnabled = true;
    bool serialCliEnabled = true;
    
    // Sub-GHz configuration
    float subGhzFrequencyMHz = 433.92f;
    SubGhzModulation subGhzMod = SubGhzModulation::MOD_CW;
    int8_t subGhzPowerDbm = 10;
    
    // Custom Pin Configuration Overrides
    PinConfig pins;
};
