/**
 * @file ChannelMath.h
 * @brief Mathematical algorithms for coprime RF hopping, channel mapping and jitter
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#pragma once

#include <stdint.h>
#include <stddef.h>

class ChannelMath {
public:
    // Greatest Common Divisor
    static uint32_t gcd(uint32_t a, uint32_t b);

    // Checks if two numbers are coprime
    static bool isCoprime(uint32_t a, uint32_t b);

    // Finds the optimal coprime step for a given channel count
    static uint8_t findBestCoprimeStep(uint8_t channelCount);

    // Calculates the next hop index in a coprime permutation sequence
    static uint8_t nextCoprimeIndex(uint8_t currentIndex, uint8_t step, uint8_t channelCount);

    // Generates Gaussian/uniform random dwell jitter in microseconds
    static uint16_t getRandomDwellUs(uint16_t minDwell, uint16_t maxDwell);

    // Frequency & Channel Conversions
    static float nrfChannelToFrequencyMHz(uint8_t channel);
    static uint8_t frequencyToNrfChannel(float freqMHz);

    static float wifiChannelToFrequencyMHz(uint8_t wifiChannel);
    static uint8_t wifiChannelToNrfChannel(uint8_t wifiChannel);

    static float bleChannelToFrequencyMHz(uint8_t bleChannel);
    static uint8_t bleChannelToNrfChannel(uint8_t bleChannel);

    static float zigbeeChannelToFrequencyMHz(uint8_t zigbeeChannel);
    static uint8_t zigbeeChannelToNrfChannel(uint8_t zigbeeChannel);
};
