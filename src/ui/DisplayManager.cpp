/**
 * @file DisplayManager.cpp
 * @brief SSD1306 OLED HUD driver implementation
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#include "ui/DisplayManager.h"
#include "core/SystemState.h"
#include "core/Logger.h"

#ifndef HOST_SIMULATION
#include <Arduino.h>
#endif

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

DisplayManager& DisplayManager::instance() {
    static DisplayManager s_instance;
    return s_instance;
}

DisplayManager::DisplayManager() {
}

DisplayManager::~DisplayManager() {
#ifndef HOST_SIMULATION
    if (display != nullptr) {
        delete display;
        display = nullptr;
    }
#endif
}

bool DisplayManager::init(const PinConfig& pins) {
    if (pins.i2c_sda < 0 || pins.i2c_scl < 0) {
        ready = false;
        return false;
    }

#ifndef HOST_SIMULATION
    Wire.begin(pins.i2c_sda, pins.i2c_scl);

    display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
    if (!display->begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        SWORD_LOGW("DISP", "SSD1306 OLED display not found on I2C address 0x%02X.", SCREEN_ADDRESS);
        ready = false;
        return false;
    }

    ready = true;
    showBootLogo();
    SWORD_LOGS("DISP", "SSD1306 OLED display initialized successfully.");
    return true;
#else
    ready = false;
    return false;
#endif
}

void DisplayManager::showBootLogo() {
#ifndef HOST_SIMULATION
    if (!ready || display == nullptr) return;

    display->clearDisplay();
    display->setTextSize(1);
    display->setTextColor(SSD1306_WHITE);
    display->setCursor(18, 12);
    display->println("ESP32-RF-SWORD");
    display->setCursor(30, 26);
    display->println("v2.5.0 PRO");
    display->setCursor(12, 44);
    display->println("INITIALIZING RF...");
    display->display();
    delay(1000);
#endif
}

void DisplayManager::updateHUD() {
#ifndef HOST_SIMULATION
    if (!ready || display == nullptr) return;

    uint32_t now = millis();
    if (now - lastFrameTime < 250) return; // 4 FPS update for low I2C overhead
    lastFrameTime = now;

    TelemetryData t = SystemState::instance().getTelemetry();

    display->clearDisplay();
    display->setTextSize(1);
    display->setTextColor(SSD1306_WHITE);

    // Header: Mode & Run State
    display->setCursor(0, 0);
    display->printf("[%s] %s", SystemState::instance().isRunning() ? "RUN" : "IDLE", SystemState::instance().getModeName(t.activeMode));

    display->drawLine(0, 9, 128, 9, SSD1306_WHITE);

    // Active Radios & Channels
    RadioStatus r0 = SystemState::instance().getRadioStatus(0);
    RadioStatus r1 = SystemState::instance().getRadioStatus(1);

    display->setCursor(0, 12);
    if (r0.present) {
        display->printf("R1: Ch%02u (%.0fM) %c", r0.currentChannel, r0.currentFrequencyMHz, r0.active ? '*' : ' ');
    } else {
        display->println("R1: OFFLINE");
    }

    display->setCursor(0, 22);
    if (r1.present) {
        display->printf("R2: Ch%02u (%.0fM) %c", r1.currentChannel, r1.currentFrequencyMHz, r1.active ? '*' : ' ');
    } else {
        display->println("R2: OFFLINE");
    }

    display->drawLine(0, 32, 128, 32, SSD1306_WHITE);

    // Performance Metrics
    display->setCursor(0, 36);
    display->printf("Hops/s: %u", t.totalHopsPerSecond);

    display->setCursor(0, 46);
    display->printf("Pkt/s : %u", t.totalPacketsPerSecond);

    // Footer
    display->setCursor(0, 56);
    display->printf("192.168.4.1 | %us", t.uptimeSeconds);

    display->display();
#endif
}

void DisplayManager::showEmergencyStop() {
#ifndef HOST_SIMULATION
    if (!ready || display == nullptr) return;

    display->clearDisplay();
    display->setTextSize(2);
    display->setTextColor(SSD1306_WHITE);
    display->setCursor(14, 20);
    display->println("RF STOPPED");
    display->display();
#endif
}

bool DisplayManager::isConnected() const {
    return ready;
}
