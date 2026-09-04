/**
 * @file WaterfallBuffer.cpp
 * @brief Circular buffer for real-time waterfall spectrum rendering implementation
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#include "analyzer/WaterfallBuffer.h"
#include <string.h>

WaterfallBuffer::WaterfallBuffer() {
    clear();
}

void WaterfallBuffer::clear() {
    memset(buffer, 0, sizeof(buffer));
    head = 0;
    count = 0;
}

void WaterfallBuffer::addScanRow(const uint8_t* rssiValues, size_t numValues) {
    if (rssiValues == nullptr) return;
    
    size_t copyLen = numValues < WATERFALL_CHANNELS ? numValues : WATERFALL_CHANNELS;
    memcpy(buffer[head], rssiValues, copyLen);
    if (copyLen < WATERFALL_CHANNELS) {
        memset(buffer[head] + copyLen, 0, WATERFALL_CHANNELS - copyLen);
    }

    head = (head + 1) % WATERFALL_HISTORY_DEPTH;
    if (count < WATERFALL_HISTORY_DEPTH) {
        count++;
    }
}

const uint8_t* WaterfallBuffer::getRow(size_t rowIndex) const {
    if (rowIndex >= count) return nullptr;
    // rowIndex = 0 is most recent row
    size_t actualIdx = (head + WATERFALL_HISTORY_DEPTH - 1 - rowIndex) % WATERFALL_HISTORY_DEPTH;
    return buffer[actualIdx];
}

size_t WaterfallBuffer::getRowCount() const {
    return count;
}

void WaterfallBuffer::getLatestRow(uint8_t* outputBuffer) const {
    if (outputBuffer == nullptr) return;
    if (count == 0) {
        memset(outputBuffer, 0, WATERFALL_CHANNELS);
        return;
    }
    size_t latestIdx = (head + WATERFALL_HISTORY_DEPTH - 1) % WATERFALL_HISTORY_DEPTH;
    memcpy(outputBuffer, buffer[latestIdx], WATERFALL_CHANNELS);
}
