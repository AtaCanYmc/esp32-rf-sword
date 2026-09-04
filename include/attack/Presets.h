/**
 * @file Presets.h
 * @brief Pre-configured frequency targets & protocol profiles
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#pragma once

#include "Types.h"
#include <stdint.h>
#include <vector>

struct TargetProfile {
    TargetPreset preset;
    const char* name;
    const char* description;
    uint8_t minChannel;
    uint8_t maxChannel;
    uint8_t channelList[32];
    uint8_t channelListLen;
    uint16_t minDwellUs;
    uint16_t maxDwellUs;
    float subGhzFreqMHz;
};

class Presets {
public:
    static const TargetProfile& getProfile(TargetPreset preset);
    static TargetPreset getPresetByName(const char* name);
    static size_t getProfileCount();
    static const TargetProfile& getProfileByIndex(size_t index);
};
