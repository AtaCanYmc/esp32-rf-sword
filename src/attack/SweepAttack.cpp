/**
 * @file SweepAttack.cpp
 * @brief Advanced Coprime Multi-Radio 2.4GHz Sweeper Engine implementation
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#include "attack/SweepAttack.h"
#include "radio/NRF24Controller.h"
#include "radio/ChannelMath.h"
#include "core/SystemState.h"
#include "core/Logger.h"

#ifndef HOST_SIMULATION
#include <Arduino.h>
#include <esp_random.h>
#endif

SweepAttack& SweepAttack::instance() {
    static SweepAttack s_instance;
    return s_instance;
}

SweepAttack::SweepAttack() {
}

void SweepAttack::init() {
    reset();
    uint8_t minCh, maxCh;
    SystemState::instance().getChannels(minCh, maxCh);
    linearCurrentCh = minCh;

    // Start carrier on all available radios with initial channels
    NRF24Controller::instance().startConstantCarrierAll(minCh, minCh + (maxCh - minCh) / 2);
}

void SweepAttack::reset() {
    sweepIndex = 0;
    linearDirectionUp = true;
}

void SweepAttack::stepCoprimeSweep() {
    uint8_t minCh, maxCh;
    SystemState::instance().getChannels(minCh, maxCh);
    uint8_t channelCount = maxCh - minCh + 1;
    if (channelCount == 0) channelCount = 1;

    // Determine coprime step
    DeviceConfig cfg = SystemState::instance().getConfig();
    uint8_t step = cfg.sweepStep;
    if (!ChannelMath::isCoprime(step, channelCount)) {
        step = ChannelMath::findBestCoprimeStep(channelCount);
    }

    uint8_t radioOffset = cfg.radioOffset;
    if (radioOffset >= channelCount) {
        radioOffset = channelCount / 2;
    }

    sweepIndex = (sweepIndex + step) % channelCount;

    // Update Radio 0
    uint8_t chA = minCh + sweepIndex;
    NRF24Controller::instance().setChannel(0, chA);
    SystemState::instance().updateRadioStatus(0, true, chA, ChannelMath::nrfChannelToFrequencyMHz(chA));
    SystemState::instance().recordHops(0, 1);

    // Update Radio 1
    uint8_t chB = minCh + ((sweepIndex + radioOffset) % channelCount);
    NRF24Controller::instance().setChannel(1, chB);
    SystemState::instance().updateRadioStatus(1, true, chB, ChannelMath::nrfChannelToFrequencyMHz(chB));
    SystemState::instance().recordHops(1, 1);

    // Update Radio 2 if present
    if (NRF24Controller::instance().isRadioConnected(2)) {
        uint8_t chC = minCh + ((sweepIndex + (radioOffset * 2)) % channelCount);
        NRF24Controller::instance().setChannel(2, chC);
        SystemState::instance().updateRadioStatus(2, true, chC, ChannelMath::nrfChannelToFrequencyMHz(chC));
        SystemState::instance().recordHops(2, 1);
    }

    // Update Radio 3 if present
    if (NRF24Controller::instance().isRadioConnected(3)) {
        uint8_t chD = minCh + ((sweepIndex + (radioOffset * 3)) % channelCount);
        NRF24Controller::instance().setChannel(3, chD);
        SystemState::instance().updateRadioStatus(3, true, chD, ChannelMath::nrfChannelToFrequencyMHz(chD));
        SystemState::instance().recordHops(3, 1);
    }

    // Gaussian jitter dwell time
    uint16_t minDwell, maxDwell;
    SystemState::instance().getDwellRangeUs(minDwell, maxDwell);
    uint16_t dwell = ChannelMath::getRandomDwellUs(minDwell, maxDwell);
#ifndef HOST_SIMULATION
    delayMicroseconds(dwell);
#endif
}

void SweepAttack::stepLinearSweep() {
    uint8_t minCh, maxCh;
    SystemState::instance().getChannels(minCh, maxCh);

    if (linearDirectionUp) {
        linearCurrentCh++;
        if (linearCurrentCh >= maxCh) {
            linearCurrentCh = maxCh;
            linearDirectionUp = false;
        }
    } else {
        if (linearCurrentCh <= minCh) {
            linearCurrentCh = minCh;
            linearDirectionUp = true;
        } else {
            linearCurrentCh--;
        }
    }

    uint8_t chA = linearCurrentCh;
    uint8_t chB = maxCh - (linearCurrentCh - minCh);

    NRF24Controller::instance().setChannel(0, chA);
    NRF24Controller::instance().setChannel(1, chB);
    SystemState::instance().updateRadioStatus(0, true, chA, ChannelMath::nrfChannelToFrequencyMHz(chA));
    SystemState::instance().updateRadioStatus(1, true, chB, ChannelMath::nrfChannelToFrequencyMHz(chB));
    SystemState::instance().recordHops(0, 1);
    SystemState::instance().recordHops(1, 1);

    uint16_t minDwell, maxDwell;
    SystemState::instance().getDwellRangeUs(minDwell, maxDwell);
    uint16_t dwell = ChannelMath::getRandomDwellUs(minDwell, maxDwell);
#ifndef HOST_SIMULATION
    delayMicroseconds(dwell);
#endif
}

void SweepAttack::stepRandomSweep() {
    uint8_t minCh, maxCh;
    SystemState::instance().getChannels(minCh, maxCh);
    uint8_t count = maxCh - minCh + 1;
    if (count == 0) count = 1;

#ifndef HOST_SIMULATION
    uint8_t chA = minCh + (esp_random() % count);
    uint8_t chB = minCh + (esp_random() % count);
#else
    uint8_t chA = minCh;
    uint8_t chB = maxCh;
#endif

    NRF24Controller::instance().setChannel(0, chA);
    NRF24Controller::instance().setChannel(1, chB);
    SystemState::instance().updateRadioStatus(0, true, chA, ChannelMath::nrfChannelToFrequencyMHz(chA));
    SystemState::instance().updateRadioStatus(1, true, chB, ChannelMath::nrfChannelToFrequencyMHz(chB));
    SystemState::instance().recordHops(0, 1);
    SystemState::instance().recordHops(1, 1);

    uint16_t minDwell, maxDwell;
    SystemState::instance().getDwellRangeUs(minDwell, maxDwell);
    uint16_t dwell = ChannelMath::getRandomDwellUs(minDwell, maxDwell);
#ifndef HOST_SIMULATION
    delayMicroseconds(dwell);
#endif
}
