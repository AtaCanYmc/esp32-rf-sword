/**
 * @file NoiseBurstAttack.cpp
 * @brief High-Power Corrupted Packet & Noise Frame Blaster Engine implementation
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#include "attack/NoiseBurstAttack.h"
#include "radio/NRF24Controller.h"
#include "radio/ChannelMath.h"
#include "core/SystemState.h"

#ifndef HOST_SIMULATION
#include <Arduino.h>
#include <esp_random.h>
#endif

NoiseBurstAttack& NoiseBurstAttack::instance() {
    static NoiseBurstAttack s_instance;
    return s_instance;
}

NoiseBurstAttack::NoiseBurstAttack() {
    for (size_t i = 0; i < sizeof(noiseBuffer); i++) {
        noiseBuffer[i] = (uint8_t)(i ^ 0xAA);
    }
}

void NoiseBurstAttack::init() {
    uint8_t minCh, maxCh;
    SystemState::instance().getChannels(minCh, maxCh);
    currentChannel = minCh;
}

void NoiseBurstAttack::stepNoiseBurst() {
    uint8_t minCh, maxCh;
    SystemState::instance().getChannels(minCh, maxCh);
    uint8_t count = maxCh - minCh + 1;
    if (count == 0) count = 1;

    currentChannel = minCh + (currentChannel + 37) % count;

#ifndef HOST_SIMULATION
    // Randomize noise payload
    for (size_t i = 0; i < sizeof(noiseBuffer); i++) {
        noiseBuffer[i] = (uint8_t)esp_random();
    }
#endif

    NRF24Controller::instance().setChannel(0, currentChannel);
    NRF24Controller::instance().sendNoiseBurst(0, noiseBuffer, sizeof(noiseBuffer));
    SystemState::instance().updateRadioStatus(0, true, currentChannel, ChannelMath::nrfChannelToFrequencyMHz(currentChannel));
    SystemState::instance().recordPackets(0, 1);

    if (NRF24Controller::instance().isRadioConnected(1)) {
        uint8_t chB = minCh + ((currentChannel + 39) % count);
        NRF24Controller::instance().setChannel(1, chB);
        NRF24Controller::instance().sendNoiseBurst(1, noiseBuffer, sizeof(noiseBuffer));
        SystemState::instance().updateRadioStatus(1, true, chB, ChannelMath::nrfChannelToFrequencyMHz(chB));
        SystemState::instance().recordPackets(1, 1);
    }

    uint16_t minDwell, maxDwell;
    SystemState::instance().getDwellRangeUs(minDwell, maxDwell);
    uint16_t dwell = ChannelMath::getRandomDwellUs(minDwell, maxDwell);
#ifndef HOST_SIMULATION
    delayMicroseconds(dwell);
#endif
}
