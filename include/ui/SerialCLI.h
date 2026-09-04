/**
 * @file SerialCLI.h
 * @brief Interactive ANSI/VT100 Serial Command Line Interface for ESP32-RF-SWORD
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#pragma once

#include "Types.h"
#include <stdint.h>
#include <stddef.h>

class SerialCLI {
public:
    static SerialCLI& instance();

    void init();
    void update();
    void printBanner();
    void printPrompt();

    void executeCommand(const char* line);

private:
    SerialCLI();
    ~SerialCLI() = default;

    char inputBuffer[128];
    size_t inputPos = 0;

    void handleHelp();
    void handleStatus();
    void handleMode(int argc, char* argv[]);
    void handlePreset(int argc, char* argv[]);
    void handleChannels(int argc, char* argv[]);
    void handleDwell(int argc, char* argv[]);
    void handlePower(int argc, char* argv[]);
    void handleSpectrum();
    void handlePinout();
    void handleSave();
    void handleReset();
    void handleReboot();
};
