/**
 * @file DisplayManager.h
 * @brief SSD1306 OLED HUD & Graphical Display Driver for ESP32-RF-SWORD
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#pragma once

#include "Types.h"
#include <stdint.h>

#ifndef HOST_SIMULATION
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#endif

class DisplayManager {
public:
    static DisplayManager& instance();

    bool init(const PinConfig& pins);
    void updateHUD();
    void showBootLogo();
    void showEmergencyStop();

    bool isConnected() const;

private:
    DisplayManager();
    ~DisplayManager();

    bool ready = false;
    uint32_t lastFrameTime = 0;

#ifndef HOST_SIMULATION
    Adafruit_SSD1306* display = nullptr;
#endif
};
