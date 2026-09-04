/**
 * @file BLEEngine.h
 * @brief ESP32 Native BLE Advertisement Flooder & Protocol Tester
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#pragma once

#include "Types.h"
#include <stdint.h>
#include <stddef.h>

enum class BLESpamType : uint8_t {
    APPLE_AIRPODS = 0,
    APPLE_AIRTAG,
    APPLE_DEVICE_POPUP,
    GOOGLE_FAST_PAIR,
    MICROSOFT_SWIFT_PAIR,
    RANDOM_CYCLE
};

class BLEEngine {
public:
    static BLEEngine& instance();

    bool init();
    void stop();

    void sendApplePopup(uint16_t deviceModel = 0x020E); // AirPods Pro default
    void sendGoogleFastPair(uint32_t modelId = 0x000143);
    void sendMicrosoftSwiftPair(const char* displayName = "Sword Research Device");

    void stepSpamCycle();

private:
    BLEEngine();
    ~BLEEngine();

    bool initialized = false;
    uint32_t spamCounter = 0;
    BLESpamType activeSpamType = BLESpamType::RANDOM_CYCLE;
};
