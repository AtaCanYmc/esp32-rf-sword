/**
 * @file SpectrumScanner.cpp
 * @brief High-Speed 2.4GHz 128-Channel Energy & Carrier Spectrum Analyzer implementation
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#include "analyzer/SpectrumScanner.h"
#include "radio/NRF24Controller.h"
#include "radio/ChannelMath.h"
#include "core/SystemState.h"
#include "core/Logger.h"
#include <string.h>

#ifndef HOST_SIMULATION
#include <Arduino.h>
#endif

SpectrumScanner& SpectrumScanner::instance() {
    static SpectrumScanner s_instance;
    return s_instance;
}

SpectrumScanner::SpectrumScanner() {
    memset(currentScan, 0, sizeof(currentScan));
}

void SpectrumScanner::init() {
    memset(currentScan, 0, sizeof(currentScan));
    waterfall.clear();
    scanPassCount = 0;
}

void SpectrumScanner::stepScan() {
    uint8_t minCh, maxCh;
    SystemState::instance().getChannels(minCh, maxCh);
    if (maxCh >= WATERFALL_CHANNELS) maxCh = WATERFALL_CHANNELS - 1;

    // Scan across channels using Radio 0 and Radio 1 in parallel
    for (uint8_t ch = minCh; ch <= maxCh; ch++) {
        int8_t rssiA = NRF24Controller::instance().scanRssi(0, ch);
        uint8_t level = (rssiA > -70) ? 100 : 0;
        
        // Decay previous value for smoother visualization
        currentScan[ch] = (uint8_t)((currentScan[ch] * 3 + level) / 4);
    }

    waterfall.addScanRow(currentScan, WATERFALL_CHANNELS);
    scanPassCount++;
    SystemState::instance().recordHops(0, maxCh - minCh + 1);
}

const WaterfallBuffer& SpectrumScanner::getWaterfall() const {
    return waterfall;
}

void SpectrumScanner::getLatestScan(uint8_t* outScan) const {
    if (outScan != nullptr) {
        memcpy(outScan, currentScan, sizeof(currentScan));
    }
}

void SpectrumScanner::printAsciiSpectrum() {
    SWORD_RAW("\r\n\033[1;36m=== 2.4GHz SPECTRUM SCANNER (Channels 0 - 80) ===\033[0m\r\n");
    for (uint8_t ch = 2; ch <= 80; ch += 2) {
        uint8_t val = currentScan[ch];
        char bar[16];
        int numStars = (val * 10) / 100;
        if (numStars > 10) numStars = 10;
        for (int i = 0; i < numStars; i++) bar[i] = '#';
        for (int i = numStars; i < 10; i++) bar[i] = '.';
        bar[10] = '\0';

        float freq = ChannelMath::nrfChannelToFrequencyMHz(ch);
        SWORD_RAW("[%02d|%4.0fMHz]: [%s] ", ch, freq, bar);
        if ((ch % 6) == 0) SWORD_RAW("\r\n");
    }
    SWORD_RAW("\r\n");
}
