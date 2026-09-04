/**
 * @file WiFiEngine.h
 * @brief ESP32 Native 802.11 Raw Frame Generator & Penetration Testing Engine
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#pragma once

#include "Types.h"
#include <stdint.h>
#include <stddef.h>

class WiFiEngine {
public:
    static WiFiEngine& instance();

    bool init();
    void stop();

    // 802.11 Raw Frame Attacks
    void sendBeaconSpam(const char* ssid, uint8_t channel);
    void sendDeauthFrame(const uint8_t* bssid, const uint8_t* targetClient, uint8_t channel, uint16_t reasonCode = 7);
    void sendDisassocFrame(const uint8_t* bssid, const uint8_t* targetClient, uint8_t channel, uint16_t reasonCode = 8);
    void sendProbeRequestSpam(const char* ssid);

    // Blast presets
    void stepBeaconStorm(uint8_t channel);
    void stepDeauthBroadcastStorm(uint8_t channel);

    void setChannel(uint8_t channel);
    uint8_t getChannel() const;

private:
    WiFiEngine();
    ~WiFiEngine();

    bool initialized = false;
    uint8_t currentChannel = 1;
    uint32_t beaconIndex = 0;
};
