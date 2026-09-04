/**
 * @file TargetedAttack.cpp
 * @brief Targeted Protocol Frequency Jammer Engine implementation
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#include "attack/TargetedAttack.h"
#include "radio/NRF24Controller.h"
#include "radio/ChannelMath.h"
#include "core/SystemState.h"
#include <string.h>

#ifndef HOST_SIMULATION
#include <Arduino.h>
#endif

TargetedAttack& TargetedAttack::instance() {
    static TargetedAttack s_instance;
    return s_instance;
}

TargetedAttack::TargetedAttack() {
}

void TargetedAttack::init(TargetPreset preset) {
    activePreset = preset;
    currentTargetIndex = 0;
    const TargetProfile& prof = Presets::getProfile(preset);

    if (prof.channelListLen > 0) {
        targetChannelCount = prof.channelListLen;
        memcpy(targetChannels, prof.channelList, targetChannelCount);
    } else {
        // Linear range from minChannel to maxChannel
        targetChannelCount = 0;
        for (uint8_t ch = prof.minChannel; ch <= prof.maxChannel && targetChannelCount < 32; ch++) {
            targetChannels[targetChannelCount++] = ch;
        }
    }

    if (targetChannelCount == 0) {
        targetChannels[0] = 2;
        targetChannelCount = 1;
    }

    SystemState::instance().setChannels(prof.minChannel, prof.maxChannel);
    SystemState::instance().setDwellRangeUs(prof.minDwellUs, prof.maxDwellUs);

    NRF24Controller::instance().startConstantCarrierAll(targetChannels[0], targetChannels[targetChannelCount > 1 ? 1 : 0]);
}

void TargetedAttack::stepTargetedJammer() {
    if (targetChannelCount == 0) return;

    currentTargetIndex = (currentTargetIndex + 1) % targetChannelCount;
    uint8_t chA = targetChannels[currentTargetIndex];
    uint8_t chB = targetChannels[(currentTargetIndex + (targetChannelCount / 2)) % targetChannelCount];

    NRF24Controller::instance().setChannel(0, chA);
    NRF24Controller::instance().setChannel(1, chB);
    SystemState::instance().updateRadioStatus(0, true, chA, ChannelMath::nrfChannelToFrequencyMHz(chA));
    SystemState::instance().updateRadioStatus(1, true, chB, ChannelMath::nrfChannelToFrequencyMHz(chB));
    SystemState::instance().recordHops(0, 1);
    SystemState::instance().recordHops(1, 1);

    if (NRF24Controller::instance().isRadioConnected(2)) {
        uint8_t chC = targetChannels[(currentTargetIndex + 1) % targetChannelCount];
        NRF24Controller::instance().setChannel(2, chC);
        SystemState::instance().updateRadioStatus(2, true, chC, ChannelMath::nrfChannelToFrequencyMHz(chC));
        SystemState::instance().recordHops(2, 1);
    }

    uint16_t minDwell, maxDwell;
    SystemState::instance().getDwellRangeUs(minDwell, maxDwell);
    uint16_t dwell = ChannelMath::getRandomDwellUs(minDwell, maxDwell);
#ifndef HOST_SIMULATION
    delayMicroseconds(dwell);
#endif
}
