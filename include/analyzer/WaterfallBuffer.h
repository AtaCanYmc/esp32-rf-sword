/**
 * @file WaterfallBuffer.h
 * @brief Circular buffer for real-time 2.4GHz waterfall spectrum rendering
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#pragma once

#include <stdint.h>
#include <stddef.h>

#define WATERFALL_CHANNELS   128
#define WATERFALL_HISTORY_DEPTH 32

class WaterfallBuffer {
public:
    WaterfallBuffer();

    void addScanRow(const uint8_t* rssiValues, size_t count);
    const uint8_t* getRow(size_t rowIndex) const;
    size_t getRowCount() const;
    void getLatestRow(uint8_t* outputBuffer) const;

    void clear();

private:
    uint8_t buffer[WATERFALL_HISTORY_DEPTH][WATERFALL_CHANNELS];
    size_t head = 0;
    size_t count = 0;
};
