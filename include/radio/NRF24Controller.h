/**
 * @file NRF24Controller.h
 * @brief High-Speed Multi-nRF24 Transceiver Controller (Dual & Quad Radio)
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#pragma once

#include "radio/RadioEngine.h"
#include "hal/PinDefinitions.h"
#include "Config.h"

#ifndef HOST_SIMULATION
#include <RF24.h>
#endif

class NRF24Controller {
public:
    static NRF24Controller& instance();

    bool init(const PinConfig& pins, uint32_t spiSpeed = DEFAULT_SPI_SPEED_HZ);
    void configureRadios(RadioPowerLevel power = RadioPowerLevel::POWER_MAX, RadioDataRate rate = RadioDataRate::RATE_2MBPS);
    
    bool isRadioConnected(uint8_t index) const;
    uint8_t getConnectedCount() const;

    void startConstantCarrierAll(uint8_t startChannelA, uint8_t startChannelB);
    void startConstantCarrier(uint8_t radioIndex, uint8_t channel);
    void stopAll();

    void setChannel(uint8_t radioIndex, uint8_t channel);
    void sendNoiseBurst(uint8_t radioIndex, const uint8_t* payload, size_t len);

    bool testCarrier(uint8_t radioIndex);
    int8_t scanRssi(uint8_t radioIndex, uint8_t channel);

    void setPowerLevel(RadioPowerLevel power);

private:
    NRF24Controller();
    ~NRF24Controller();

#ifndef HOST_SIMULATION
    RF24* radios[MAX_NRF24_MODULES];
#endif
    bool radioReady[MAX_NRF24_MODULES];
    uint8_t connectedCount = 0;
    RadioPowerLevel currentPower = RadioPowerLevel::POWER_MAX;
    RadioDataRate currentRate = RadioDataRate::RATE_2MBPS;
    PinConfig activePins;
};
