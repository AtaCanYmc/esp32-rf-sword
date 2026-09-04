/**
 * @file BoardProfiles.h
 * @brief Board profile auto-detection and hardware configuration manager
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#pragma once

#include "Types.h"
#include "hal/PinDefinitions.h"

class BoardProfiles {
public:
    static BoardType detectBoard();
    static PinConfig getEffectivePins(const DeviceConfig& cfg);
    static void printBoardInfo(BoardType type, const PinConfig& pins);
};
