/**
 * @file WiFiDeauthAttack.h
 * @brief Native 802.11 Deauthentication & Beacon Flood Attack Coordinator
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#pragma once

#include "Types.h"
#include <stdint.h>

class WiFiDeauthAttack {
public:
    static WiFiDeauthAttack& instance();

    void init();
    void stepDeauthStorm();
    void stepBeaconFlood();

private:
    WiFiDeauthAttack();
    ~WiFiDeauthAttack() = default;

    uint8_t currentChannel = 1;
};
