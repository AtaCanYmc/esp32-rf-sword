/**
 * @file ChannelMath.cpp
 * @brief Mathematical algorithms for coprime hopping and channel mapping implementation
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#include "radio/ChannelMath.h"

#ifndef HOST_SIMULATION
#include <Arduino.h>
#include <esp_random.h>
#else
#include <random>
static std::mt19937 rng(42);
#endif

uint32_t ChannelMath::gcd(uint32_t a, uint32_t b) {
    while (b != 0) {
        uint32_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

bool ChannelMath::isCoprime(uint32_t a, uint32_t b) {
    return gcd(a, b) == 1;
}

uint8_t ChannelMath::findBestCoprimeStep(uint8_t channelCount) {
    if (channelCount <= 1) return 1;
    
    // Choose a step near the golden ratio approximation of channelCount
    uint8_t target = (channelCount * 618) / 1000;
    if (target < 2) target = channelCount / 2;

    for (uint8_t offset = 0; offset < channelCount; offset++) {
        if (target + offset < channelCount && isCoprime(target + offset, channelCount)) {
            return target + offset;
        }
        if (target >= offset && target - offset > 1 && isCoprime(target - offset, channelCount)) {
            return target - offset;
        }
    }
    return 1;
}

uint8_t ChannelMath::nextCoprimeIndex(uint8_t currentIndex, uint8_t step, uint8_t channelCount) {
    if (channelCount == 0) return 0;
    return (currentIndex + step) % channelCount;
}

uint16_t ChannelMath::getRandomDwellUs(uint16_t minDwell, uint16_t maxDwell) {
    if (minDwell >= maxDwell) return minDwell;

#ifndef HOST_SIMULATION
    uint32_t r = esp_random();
    return minDwell + (r % (maxDwell - minDwell + 1));
#else
    std::uniform_int_distribution<uint16_t> dist(minDwell, maxDwell);
    return dist(rng);
#endif
}

float ChannelMath::nrfChannelToFrequencyMHz(uint8_t channel) {
    return 2400.0f + (float)channel;
}

uint8_t ChannelMath::frequencyToNrfChannel(float freqMHz) {
    if (freqMHz < 2400.0f) return 0;
    if (freqMHz > 2525.0f) return 125;
    return (uint8_t)(freqMHz - 2400.0f + 0.5f);
}

float ChannelMath::wifiChannelToFrequencyMHz(uint8_t wifiChannel) {
    if (wifiChannel >= 1 && wifiChannel <= 13) {
        return 2412.0f + (wifiChannel - 1) * 5.0f;
    } else if (wifiChannel == 14) {
        return 2484.0f;
    }
    return 2412.0f;
}

uint8_t ChannelMath::wifiChannelToNrfChannel(uint8_t wifiChannel) {
    float freq = wifiChannelToFrequencyMHz(wifiChannel);
    return frequencyToNrfChannel(freq);
}

float ChannelMath::bleChannelToFrequencyMHz(uint8_t bleChannel) {
    if (bleChannel == 37) return 2402.0f;
    if (bleChannel == 38) return 2426.0f;
    if (bleChannel == 39) return 2480.0f;
    if (bleChannel <= 10) {
        return 2404.0f + (bleChannel * 2.0f);
    } else if (bleChannel <= 36) {
        return 2428.0f + ((bleChannel - 11) * 2.0f);
    }
    return 2402.0f;
}

uint8_t ChannelMath::bleChannelToNrfChannel(uint8_t bleChannel) {
    float freq = bleChannelToFrequencyMHz(bleChannel);
    return frequencyToNrfChannel(freq);
}

float ChannelMath::zigbeeChannelToFrequencyMHz(uint8_t zigbeeChannel) {
    if (zigbeeChannel >= 11 && zigbeeChannel <= 26) {
        return 2405.0f + (zigbeeChannel - 11) * 5.0f;
    }
    return 2405.0f;
}

uint8_t ChannelMath::zigbeeChannelToNrfChannel(uint8_t zigbeeChannel) {
    float freq = zigbeeChannelToFrequencyMHz(zigbeeChannel);
    return frequencyToNrfChannel(freq);
}
