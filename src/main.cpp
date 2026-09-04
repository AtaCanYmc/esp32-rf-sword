/**
 * @file main.cpp
 * @brief Application Entry Point for ESP32-RF-SWORD
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#include <Arduino.h>
#include <esp_system.h>
#include "Config.h"
#include "Types.h"
#include "core/Logger.h"
#include "core/SystemState.h"
#include "core/NVSManager.h"
#include "core/TaskManager.h"
#include "hal/BoardProfiles.h"
#include "hal/SPIManager.h"
#include "radio/NRF24Controller.h"
#include "radio/CC1101Controller.h"
#include "attack/AttackCoordinator.h"
#include "ui/WebServerManager.h"
#include "ui/DisplayManager.h"
#include "ui/SerialCLI.h"

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    
    // Allow USB CDC on ESP32-C3/S3 to stabilize
    delay(1500);

    Logger::init(LogLevel::LOG_INFO);
    SWORD_LOGI("BOOT", "Initializing %s v%s...", SWORD_PROJECT_NAME, SWORD_VERSION);

    // Initialize System State & Flash NVS
    SystemState::instance().init();
    NVSManager::init();

    DeviceConfig config;
    NVSManager::loadConfig(config);
    SystemState::instance().setConfig(config);

    // Detect hardware board profile and active GPIO pins
    BoardType board = BoardProfiles::detectBoard();
    SystemState::instance().setBoardType(board);
    PinConfig pins = BoardProfiles::getEffectivePins(config);
    BoardProfiles::printBoardInfo(board, pins);

    // Initialize Hardware Peripherals
    DisplayManager::instance().init(pins);

    SWORD_LOGI("BOOT", "Initializing 2.4GHz nRF24L01+ radio array...");
    bool nrfOk = NRF24Controller::instance().init(pins, config.spiSpeedHz);
    if (!nrfOk) {
        SWORD_LOGW("BOOT", "No nRF24 modules responded! Check power supply (3.3V) and SPI pinout.");
    }

    // Initialize Sub-GHz CC1101 if pin defined
    if (pins.cc1101_csn >= 0) {
        SWORD_LOGI("BOOT", "Initializing Sub-GHz CC1101 transceiver...");
        CC1101Controller::instance().init(pins, config.subGhzFrequencyMHz);
    }

    // Initialize Web Dashboard & Wi-Fi Captive Portal
    if (config.wifiApEnabled) {
        WebServerManager::instance().init(config);
    }

    // Initialize Attack Coordinator
    AttackCoordinator::instance().init();

    // Start FreeRTOS Multi-Core Workers
    TaskManager::instance().startTasks();

    // Initialize Interactive Serial CLI
    SerialCLI::instance().init();

    SWORD_LOGS("BOOT", "%s ready. Status: %s", SWORD_PROJECT_NAME, SystemState::instance().isRunning() ? "ACTIVE" : "IDLE");
}

void loop() {
    // Serial CLI runs on main task
    SerialCLI::instance().update();
    delay(10);
}
