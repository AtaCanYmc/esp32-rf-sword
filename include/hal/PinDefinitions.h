/**
 * @file PinDefinitions.h
 * @brief Default Hardware Pinout Profiles for all supported ESP32 architectures
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#pragma once

#include "Types.h"

namespace BoardPins {

// ==============================================================================
// Profile 1: ESP32-C3 SuperMini (Headless / Ultra-Compact Form Factor)
// ==============================================================================
inline PinConfig getC3SuperMiniPins() {
    PinConfig p;
    p.sck  = 4;
    p.miso = 5;
    p.mosi = 6;
    
    // Dual nRF24L01+ Radios
    p.nrf_ce[0]  = 3;
    p.nrf_csn[0] = 7;
    p.nrf_ce[1]  = 1;
    p.nrf_csn[1] = 10;
    
    // Sub-GHz CC1101 Module (Optional)
    p.cc1101_csn  = 0;
    p.cc1101_gdo0 = 2;
    p.cc1101_gdo2 = 8;
    
    // I2C Display (Optional)
    p.i2c_sda = 8;
    p.i2c_scl = 9;
    
    // Status indicator
    p.status_led = 8; // Built-in Blue LED on C3 SuperMini (Active LOW)
    p.action_button = 9; // Boot Button
    return p;
}

// ==============================================================================
// Profile 2: ESP32 DevKit V1 / WROOM-32 (Standard Dual-Core Classic)
// ==============================================================================
inline PinConfig getDevKitV1Pins() {
    PinConfig p;
    p.sck  = 18;
    p.miso = 19;
    p.mosi = 23;
    
    // Quad nRF24L01+ Radios
    p.nrf_ce[0]  = 4;
    p.nrf_csn[0] = 5;
    p.nrf_ce[1]  = 2;
    p.nrf_csn[1] = 15;
    p.nrf_ce[2]  = 13;
    p.nrf_csn[2] = 12;
    p.nrf_ce[3]  = 14;
    p.nrf_csn[3] = 27;
    
    // Sub-GHz CC1101 Module
    p.cc1101_csn  = 26;
    p.cc1101_gdo0 = 25;
    p.cc1101_gdo2 = 33;
    
    // I2C OLED SSD1306
    p.i2c_sda = 21;
    p.i2c_scl = 22;
    
    p.status_led = 2; // Built-in Blue LED
    p.action_button = 0; // BOOT button
    return p;
}

// ==============================================================================
// Profile 3: ESP32-S3 DevKit (Dual-Core LX7 High-Power & USB OTG)
// ==============================================================================
inline PinConfig getS3DevKitPins() {
    PinConfig p;
    p.sck  = 12;
    p.miso = 13;
    p.mosi = 11;
    
    // Quad nRF24 Radios
    p.nrf_ce[0]  = 10;
    p.nrf_csn[0] = 9;
    p.nrf_ce[1]  = 4;
    p.nrf_csn[1] = 5;
    p.nrf_ce[2]  = 6;
    p.nrf_csn[2] = 7;
    p.nrf_ce[3]  = 15;
    p.nrf_csn[3] = 16;
    
    // Sub-GHz CC1101 Module
    p.cc1101_csn  = 17;
    p.cc1101_gdo0 = 18;
    p.cc1101_gdo2 = 8;
    
    // I2C OLED
    p.i2c_sda = 1;
    p.i2c_scl = 2;
    
    p.status_led = 48; // RGB or GPIO 21
    p.action_button = 0;
    return p;
}

// ==============================================================================
// Profile 4: ESP32-C6 DevKit (Wi-Fi 6 / 802.15.4 Native Transceiver)
// ==============================================================================
inline PinConfig getC6DevKitPins() {
    PinConfig p;
    p.sck  = 6;
    p.miso = 2;
    p.mosi = 7;
    
    p.nrf_ce[0]  = 19;
    p.nrf_csn[0] = 20;
    p.nrf_ce[1]  = 21;
    p.nrf_csn[1] = 22;
    
    p.cc1101_csn  = 23;
    p.cc1101_gdo0 = 15;
    p.cc1101_gdo2 = 18;
    
    p.i2c_sda = 8;
    p.i2c_scl = 9;
    
    p.status_led = 15;
    p.action_button = 9;
    return p;
}

} // namespace BoardPins
