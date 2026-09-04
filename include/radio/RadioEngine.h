/**
 * @file RadioEngine.h
 * @brief Base Abstract Radio Interface for Transceivers
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#pragma once

#include "Types.h"
#include <stdint.h>

class RadioEngine {
public:
    virtual ~RadioEngine() = default;

    virtual bool begin() = 0;
    virtual void stop() = 0;
    virtual bool isConnected() = 0;

    virtual void setPower(RadioPowerLevel power) = 0;
    virtual void setChannel(uint8_t channel) = 0;
    virtual void startCarrier(uint8_t channel) = 0;
    virtual void stopCarrier() = 0;
    
    virtual void sendNoisePacket(const uint8_t* data, size_t len) = 0;
    virtual bool testCarrier() = 0;
};
