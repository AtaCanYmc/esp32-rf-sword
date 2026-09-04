/**
 * @file BoardProfiles.cpp
 * @brief Board profile auto-detection implementation
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#include "hal/BoardProfiles.h"
#include "core/Logger.h"

BoardType BoardProfiles::detectBoard() {
#if defined(BOARD_ESP32_C3_SUPERMINI) || defined(CONFIG_IDF_TARGET_ESP32C3)
    return BoardType::ESP32_C3_SUPERMINI;
#elif defined(BOARD_ESP32_S3_DEVKIT) || defined(CONFIG_IDF_TARGET_ESP32S3)
    return BoardType::ESP32_S3_DEVKIT;
#elif defined(BOARD_ESP32_C6_DEVKIT) || defined(CONFIG_IDF_TARGET_ESP32C6)
    return BoardType::ESP32_C6_DEVKIT;
#elif defined(BOARD_ESP32_DEVKIT_V1) || defined(CONFIG_IDF_TARGET_ESP32)
    return BoardType::ESP32_DEVKIT_V1;
#else
    return BoardType::ESP32_DEVKIT_V1;
#endif
}

PinConfig BoardProfiles::getEffectivePins(const DeviceConfig& cfg) {
    // If custom pins are configured (SCK != -1 and MOSI != -1), use custom
    if (cfg.pins.sck != -1 && cfg.pins.mosi != -1) {
        return cfg.pins;
    }

    BoardType b = detectBoard();
    switch (b) {
        case BoardType::ESP32_C3_SUPERMINI: return BoardPins::getC3SuperMiniPins();
        case BoardType::ESP32_S3_DEVKIT:    return BoardPins::getS3DevKitPins();
        case BoardType::ESP32_C6_DEVKIT:    return BoardPins::getC6DevKitPins();
        case BoardType::ESP32_DEVKIT_V1:
        default:                            return BoardPins::getDevKitV1Pins();
    }
}

void BoardProfiles::printBoardInfo(BoardType type, const PinConfig& pins) {
    SWORD_LOGI("BOARD", "==================================================");
    switch (type) {
        case BoardType::ESP32_C3_SUPERMINI:
            SWORD_LOGI("BOARD", "Detected Hardware : ESP32-C3 SuperMini (RISC-V 160MHz)");
            break;
        case BoardType::ESP32_S3_DEVKIT:
            SWORD_LOGI("BOARD", "Detected Hardware : ESP32-S3 DevKit (Dual-Core LX7 240MHz)");
            break;
        case BoardType::ESP32_C6_DEVKIT:
            SWORD_LOGI("BOARD", "Detected Hardware : ESP32-C6 DevKit (Wi-Fi 6 + Zigbee Native)");
            break;
        case BoardType::ESP32_DEVKIT_V1:
        default:
            SWORD_LOGI("BOARD", "Detected Hardware : ESP32 DevKit V1 (Dual-Core LX6 240MHz)");
            break;
    }
    SWORD_LOGI("BOARD", "SPI Bus           : SCK=%d, MISO=%d, MOSI=%d", pins.sck, pins.miso, pins.mosi);
    SWORD_LOGI("BOARD", "nRF24 Radio #1    : CE=%d, CSN=%d", pins.nrf_ce[0], pins.nrf_csn[0]);
    SWORD_LOGI("BOARD", "nRF24 Radio #2    : CE=%d, CSN=%d", pins.nrf_ce[1], pins.nrf_csn[1]);
    if (pins.nrf_ce[2] != -1) {
        SWORD_LOGI("BOARD", "nRF24 Radio #3    : CE=%d, CSN=%d", pins.nrf_ce[2], pins.nrf_csn[2]);
    }
    if (pins.nrf_ce[3] != -1) {
        SWORD_LOGI("BOARD", "nRF24 Radio #4    : CE=%d, CSN=%d", pins.nrf_ce[3], pins.nrf_csn[3]);
    }
    if (pins.cc1101_csn != -1) {
        SWORD_LOGI("BOARD", "CC1101 Sub-GHz    : CSN=%d, GDO0=%d, GDO2=%d", pins.cc1101_csn, pins.cc1101_gdo0, pins.cc1101_gdo2);
    }
    if (pins.i2c_sda != -1) {
        SWORD_LOGI("BOARD", "I2C Display (OLED): SDA=%d, SCL=%d", pins.i2c_sda, pins.i2c_scl);
    }
    SWORD_LOGI("BOARD", "==================================================");
}
