/**
 * @file SerialCLI.cpp
 * @brief Interactive ANSI/VT100 Serial Command Line Interface implementation
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#include "ui/SerialCLI.h"
#include "core/SystemState.h"
#include "core/NVSManager.h"
#include "core/Logger.h"
#include "hal/BoardProfiles.h"
#include "radio/ChannelMath.h"
#include "attack/AttackCoordinator.h"
#include "attack/Presets.h"
#include "analyzer/SpectrumScanner.h"
#include <string.h>
#include <stdlib.h>

#ifndef HOST_SIMULATION
#include <Arduino.h>
#endif

SerialCLI& SerialCLI::instance() {
    static SerialCLI s_instance;
    return s_instance;
}

SerialCLI::SerialCLI() {
    inputPos = 0;
    inputBuffer[0] = '\0';
}

void SerialCLI::init() {
    printBanner();
    printPrompt();
}

void SerialCLI::printBanner() {
    SWORD_RAW("\r\n");
    SWORD_RAW("\033[1;36m  ______  _____ _____ ____ ___     ____  _____       ______          ______  ____  ____ \033[0m\r\n");
    SWORD_RAW("\033[1;36m |  ____|/ ____|  __ \\___ \\__ \\   |  _ \\|  ___|     / _____|\\      /|/  __  \\|  _ \\|  _ \\\033[0m\r\n");
    SWORD_RAW("\033[1;36m | |__  | (___ | |__) |__) | ) |  | |_) | |_ _____  \\_____ \\ \\ \\/\\ / /| |  | | |_) | | | |\033[0m\r\n");
    SWORD_RAW("\033[1;36m |  __|  \\___ \\|  ___/|__ < / /   |  _ <|  _/ ____|  _____) \\ \\V  V / | |  | |  _ <| |_| |\033[0m\r\n");
    SWORD_RAW("\033[1;36m | |____ ____) | |    ___) / /_   | |_) | | |       |______/ \\_/\\_/  | |__| | | \\ \\|____/ \033[0m\r\n");
    SWORD_RAW("\033[1;36m |______|_____/|_|   |____/____|  |____/|_|                           \\______/|_|  \\_\\     \033[0m\r\n");
    SWORD_RAW("\r\n");
    SWORD_RAW("\033[1;32m [ ESP32-RF-SWORD // Multi-Band RF Security & Research Toolkit v%s ]\033[0m\r\n", SWORD_VERSION);
    SWORD_RAW("\033[1;37m Type 'help' to see available commands or 'status' for real-time telemetry.\033[0m\r\n\r\n");
}

void SerialCLI::printPrompt() {
    SWORD_RAW("\033[1;32msword\033[0m \033[1;36m>\033[0m ");
}

void SerialCLI::update() {
#ifndef HOST_SIMULATION
    while (Serial.available() > 0) {
        char c = (char)Serial.read();
        if (c == '\r' || c == '\n') {
            if (inputPos > 0) {
                inputBuffer[inputPos] = '\0';
                SWORD_RAW("\r\n");
                executeCommand(inputBuffer);
                inputPos = 0;
                printPrompt();
            }
        } else if (c == 0x08 || c == 0x7F) { // Backspace
            if (inputPos > 0) {
                inputPos--;
                SWORD_RAW("\b \b");
            }
        } else if (c >= 32 && c <= 126) { // Printable characters
            if (inputPos < sizeof(inputBuffer) - 1) {
                inputBuffer[inputPos++] = c;
                Serial.write(c); // Local echo
            }
        }
    }
#endif
}

void SerialCLI::executeCommand(const char* line) {
    if (line == nullptr || strlen(line) == 0) return;

    // Tokenize
    char buffer[128];
    strncpy(buffer, line, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    char* argv[8];
    int argc = 0;

    char* token = strtok(buffer, " ");
    while (token != nullptr && argc < 8) {
        argv[argc++] = token;
        token = strtok(nullptr, " ");
    }

    if (argc == 0) return;

    if (strcasecmp(argv[0], "help") == 0) {
        handleHelp();
    } else if (strcasecmp(argv[0], "status") == 0) {
        handleStatus();
    } else if (strcasecmp(argv[0], "start") == 0) {
        SystemState::instance().start();
        SWORD_LOGS("CLI", "RF transmission started.");
    } else if (strcasecmp(argv[0], "stop") == 0) {
        AttackCoordinator::instance().stop();
        SWORD_LOGW("CLI", "RF transmission stopped.");
    } else if (strcasecmp(argv[0], "mode") == 0) {
        handleMode(argc, argv);
    } else if (strcasecmp(argv[0], "preset") == 0) {
        handlePreset(argc, argv);
    } else if (strcasecmp(argv[0], "channels") == 0) {
        handleChannels(argc, argv);
    } else if (strcasecmp(argv[0], "dwell") == 0) {
        handleDwell(argc, argv);
    } else if (strcasecmp(argv[0], "power") == 0) {
        handlePower(argc, argv);
    } else if (strcasecmp(argv[0], "spectrum") == 0) {
        handleSpectrum();
    } else if (strcasecmp(argv[0], "pinout") == 0) {
        handlePinout();
    } else if (strcasecmp(argv[0], "save") == 0) {
        handleSave();
    } else if (strcasecmp(argv[0], "reset") == 0) {
        handleReset();
    } else if (strcasecmp(argv[0], "reboot") == 0) {
        handleReboot();
    } else {
        SWORD_LOGE("CLI", "Unknown command: '%s'. Type 'help' for command list.", argv[0]);
    }
}

void SerialCLI::handleHelp() {
    SWORD_RAW("\033[1;33m=== ESP32-RF-SWORD COMMAND REFERENCE ===\033[0m\r\n");
    SWORD_RAW("  \033[1;36mstatus\033[0m                          Display system telemetry and RF status\r\n");
    SWORD_RAW("  \033[1;36mstart\033[0m                           Start active transmission/sweeper\r\n");
    SWORD_RAW("  \033[1;36mstop\033[0m                            Halt all RF transmission\r\n");
    SWORD_RAW("  \033[1;36mmode <name>\033[0m                     Set attack mode (coprime, linear, random, ble, wifi, zigbee, drone, subghz, noise, spectrum, beacon, deauth, popup, blitz)\r\n");
    SWORD_RAW("  \033[1;36mpreset <name>\033[0m                   Apply target preset (full, ble-adv, ble-all, wifi-1, wifi-6, wifi-11, zigbee, flysky, frsky, elrs, sub-315, sub-433, sub-868, sub-915)\r\n");
    SWORD_RAW("  \033[1;36mchannels <min> <max>\033[0m            Set custom RF channel range (0-125)\r\n");
    SWORD_RAW("  \033[1;36mdwell <min_us> <max_us>\033[0m         Set dwell timing jitter in microseconds\r\n");
    SWORD_RAW("  \033[1;36mpower <min|low|high|max>\033[0m        Set nRF24 output power\r\n");
    SWORD_RAW("  \033[1;36mspectrum\033[0m                        Render live 2.4GHz ASCII spectrum graph\r\n");
    SWORD_RAW("  \033[1;36mpinout\033[0m                          Show active GPIO pin assignments\r\n");
    SWORD_RAW("  \033[1;36msave\033[0m                            Save current parameters to Flash NVS\r\n");
    SWORD_RAW("  \033[1;36mreset\033[0m                           Restore factory default settings\r\n");
    SWORD_RAW("  \033[1;36mreboot\033[0m                          Restart microcontroller\r\n\r\n");
}

void SerialCLI::handleStatus() {
    TelemetryData t = SystemState::instance().getTelemetry();
    SWORD_RAW("\033[1;33m=== SYSTEM TELEMETRY & STATUS ===\033[0m\r\n");
    SWORD_RAW("  State          : %s\r\n", SystemState::instance().isRunning() ? "\033[1;32mRUNNING\033[0m" : "\033[1;31mSTOPPED\033[0m");
    SWORD_RAW("  Active Mode    : \033[1;36m%s\033[0m\r\n", SystemState::instance().getModeName(t.activeMode));
    SWORD_RAW("  Active Preset  : %s\r\n", SystemState::instance().getPresetName(t.activePreset));
    SWORD_RAW("  Hop Rate       : %u hops/sec (Total: %u)\r\n", t.totalHopsPerSecond, t.totalHopsAllTime);
    SWORD_RAW("  Packet Rate    : %u packets/sec (Total: %u)\r\n", t.totalPacketsPerSecond, t.totalPacketsAllTime);
    SWORD_RAW("  RF Power       : %s\r\n", SystemState::instance().getPowerLevelName(t.powerLevel));
    SWORD_RAW("  Channel Range  : Ch %u (%4.0f MHz) -> Ch %u (%4.0f MHz)\r\n", 
              t.minChannel, ChannelMath::nrfChannelToFrequencyMHz(t.minChannel),
              t.maxChannel, ChannelMath::nrfChannelToFrequencyMHz(t.maxChannel));
    SWORD_RAW("  Dwell Jitter   : %u - %u us\r\n", t.minDwellUs, t.maxDwellUs);
    SWORD_RAW("  Active Radios  : %u connected\r\n", t.activeRadioCount);
    for (int i = 0; i < MAX_NRF24_MODULES; i++) {
        RadioStatus st = SystemState::instance().getRadioStatus(i);
        if (st.present) {
            SWORD_RAW("    Radio #%d: Active=%s, Ch=%u (%.1f MHz), TotalHops=%u\r\n", 
                      i + 1, st.active ? "YES" : "NO", st.currentChannel, st.currentFrequencyMHz, st.channelHops);
        }
    }
    SWORD_RAW("  Uptime / Heap  : %u sec | Free Heap: %u bytes (Min: %u)\r\n\r\n", t.uptimeSeconds, t.freeHeapBytes, t.minFreeHeapBytes);
}

void SerialCLI::handleMode(int argc, char* argv[]) {
    if (argc < 2) {
        SWORD_LOGW("CLI", "Usage: mode <coprime|linear|random|ble|wifi|zigbee|drone|subghz|noise|spectrum|beacon|deauth|popup|blitz>");
        return;
    }

    const char* m = argv[1];
    if (strcasecmp(m, "coprime") == 0) AttackCoordinator::instance().setMode(AttackMode::SWEEP_COPRIME);
    else if (strcasecmp(m, "linear") == 0) AttackCoordinator::instance().setMode(AttackMode::SWEEP_LINEAR);
    else if (strcasecmp(m, "random") == 0) AttackCoordinator::instance().setMode(AttackMode::SWEEP_RANDOM);
    else if (strcasecmp(m, "ble") == 0) AttackCoordinator::instance().setMode(AttackMode::TARGETED_BLE);
    else if (strcasecmp(m, "wifi") == 0) AttackCoordinator::instance().setMode(AttackMode::TARGETED_WIFI);
    else if (strcasecmp(m, "zigbee") == 0) AttackCoordinator::instance().setMode(AttackMode::TARGETED_ZIGBEE);
    else if (strcasecmp(m, "drone") == 0) AttackCoordinator::instance().setMode(AttackMode::TARGETED_DRONE);
    else if (strcasecmp(m, "subghz") == 0) AttackCoordinator::instance().setMode(AttackMode::TARGETED_SUBGHZ);
    else if (strcasecmp(m, "noise") == 0) AttackCoordinator::instance().setMode(AttackMode::NOISE_BURST);
    else if (strcasecmp(m, "spectrum") == 0) AttackCoordinator::instance().setMode(AttackMode::SPECTRUM_ANALYZER);
    else if (strcasecmp(m, "beacon") == 0) AttackCoordinator::instance().setMode(AttackMode::WIFI_BEACON_SPAM);
    else if (strcasecmp(m, "deauth") == 0) AttackCoordinator::instance().setMode(AttackMode::WIFI_DEAUTH_STORM);
    else if (strcasecmp(m, "popup") == 0) AttackCoordinator::instance().setMode(AttackMode::BLE_POPUP_SPAM);
    else if (strcasecmp(m, "blitz") == 0) AttackCoordinator::instance().setMode(AttackMode::TOTAL_BLITZ);
    else {
        SWORD_LOGE("CLI", "Invalid mode '%s'.", m);
    }
}

void SerialCLI::handlePreset(int argc, char* argv[]) {
    if (argc < 2) {
        SWORD_LOGW("CLI", "Usage: preset <name>");
        return;
    }
    TargetPreset p = Presets::getPresetByName(argv[1]);
    AttackCoordinator::instance().applyPreset(p);
}

void SerialCLI::handleChannels(int argc, char* argv[]) {
    if (argc < 3) {
        SWORD_LOGW("CLI", "Usage: channels <min_ch> <max_ch>");
        return;
    }
    uint8_t minCh = (uint8_t)atoi(argv[1]);
    uint8_t maxCh = (uint8_t)atoi(argv[2]);
    SystemState::instance().setChannels(minCh, maxCh);
    SWORD_LOGS("CLI", "Channel range set to %u - %u", minCh, maxCh);
}

void SerialCLI::handleDwell(int argc, char* argv[]) {
    if (argc < 3) {
        SWORD_LOGW("CLI", "Usage: dwell <min_us> <max_us>");
        return;
    }
    uint16_t minD = (uint16_t)atoi(argv[1]);
    uint16_t maxD = (uint16_t)atoi(argv[2]);
    SystemState::instance().setDwellRangeUs(minD, maxD);
    SWORD_LOGS("CLI", "Dwell range set to %u - %u us", minD, maxD);
}

void SerialCLI::handlePower(int argc, char* argv[]) {
    if (argc < 2) {
        SWORD_LOGW("CLI", "Usage: power <min|low|high|max>");
        return;
    }
    const char* p = argv[1];
    if (strcasecmp(p, "min") == 0) SystemState::instance().setPowerLevel(RadioPowerLevel::POWER_MIN);
    else if (strcasecmp(p, "low") == 0) SystemState::instance().setPowerLevel(RadioPowerLevel::POWER_LOW);
    else if (strcasecmp(p, "high") == 0) SystemState::instance().setPowerLevel(RadioPowerLevel::POWER_HIGH);
    else if (strcasecmp(p, "max") == 0) SystemState::instance().setPowerLevel(RadioPowerLevel::POWER_MAX);
    else SWORD_LOGE("CLI", "Invalid power level '%s'", p);
}

void SerialCLI::handleSpectrum() {
    SpectrumScanner::instance().printAsciiSpectrum();
}

void SerialCLI::handlePinout() {
    DeviceConfig cfg = SystemState::instance().getConfig();
    PinConfig pins = BoardProfiles::getEffectivePins(cfg);
    BoardProfiles::printBoardInfo(BoardProfiles::detectBoard(), pins);
}

void SerialCLI::handleSave() {
    DeviceConfig cfg = SystemState::instance().getConfig();
    if (NVSManager::saveConfig(cfg)) {
        SWORD_LOGS("CLI", "Configuration saved to flash memory.");
    } else {
        SWORD_LOGE("CLI", "Failed to save configuration to flash.");
    }
}

void SerialCLI::handleReset() {
    DeviceConfig cfg;
    NVSManager::resetToDefaults(cfg);
    NVSManager::saveConfig(cfg);
    SystemState::instance().setConfig(cfg);
    SWORD_LOGS("CLI", "Factory defaults restored.");
}

void SerialCLI::handleReboot() {
    SWORD_LOGW("CLI", "Rebooting ESP32...");
#ifndef HOST_SIMULATION
    ESP.restart();
#endif
}
