/**
 * @file SpectrumScanner.h
 * @brief High-Speed 2.4GHz 128-Channel Energy & Carrier Spectrum Analyzer
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#pragma once

#include "analyzer/WaterfallBuffer.h"
#include <stdint.h>

class SpectrumScanner {
public:
    static SpectrumScanner& instance();

    void init();
    void stepScan();

    const WaterfallBuffer& getWaterfall() const;
    void getLatestScan(uint8_t* outScan) const;
    void printAsciiSpectrum();

private:
    SpectrumScanner();
    ~SpectrumScanner() = default;

    WaterfallBuffer waterfall;
    uint8_t currentScan[WATERFALL_CHANNELS];
    uint32_t scanPassCount = 0;
};
