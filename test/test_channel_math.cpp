/**
 * @file test_channel_math.cpp
 * @brief Unit tests for ChannelMath algorithms
 */

#include <unity.h>
#include "radio/ChannelMath.h"
#include <vector>
#include <set>

void test_gcd() {
    TEST_ASSERT_EQUAL_UINT32(1, ChannelMath::gcd(37, 79));
    TEST_ASSERT_EQUAL_UINT32(5, ChannelMath::gcd(15, 20));
    TEST_ASSERT_EQUAL_UINT32(1, ChannelMath::gcd(13, 27));
    TEST_ASSERT_EQUAL_UINT32(12, ChannelMath::gcd(24, 36));
}

void test_coprime_detection() {
    TEST_ASSERT_TRUE(ChannelMath::isCoprime(37, 79));
    TEST_ASSERT_TRUE(ChannelMath::isCoprime(17, 80));
    TEST_ASSERT_FALSE(ChannelMath::isCoprime(20, 80));
    TEST_ASSERT_FALSE(ChannelMath::isCoprime(6, 9));
}

void test_coprime_permutation_completeness() {
    // Verify that hopping with step=37 across 79 channels visits all 79 channels exactly once
    const uint8_t channelCount = 79;
    const uint8_t step = 37;
    std::set<uint8_t> visitedChannels;

    uint8_t current = 0;
    for (uint8_t i = 0; i < channelCount; i++) {
        visitedChannels.insert(current);
        current = ChannelMath::nextCoprimeIndex(current, step, channelCount);
    }

    // All 79 unique channels must be visited
    TEST_ASSERT_EQUAL_UINT32(channelCount, visitedChannels.size());
}

void test_channel_frequency_conversions() {
    // 2.4 GHz NRF channel to MHz
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 2402.0f, ChannelMath::nrfChannelToFrequencyMHz(2));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 2480.0f, ChannelMath::nrfChannelToFrequencyMHz(80));

    // Wi-Fi Channels
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 2412.0f, ChannelMath::wifiChannelToFrequencyMHz(1));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 2437.0f, ChannelMath::wifiChannelToFrequencyMHz(6));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 2462.0f, ChannelMath::wifiChannelToFrequencyMHz(11));

    // BLE Channels (37 -> 2402, 38 -> 2426, 39 -> 2480)
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 2402.0f, ChannelMath::bleChannelToFrequencyMHz(37));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 2426.0f, ChannelMath::bleChannelToFrequencyMHz(38));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 2480.0f, ChannelMath::bleChannelToFrequencyMHz(39));
    TEST_ASSERT_EQUAL_UINT8(2, ChannelMath::bleChannelToNrfChannel(37));
    TEST_ASSERT_EQUAL_UINT8(26, ChannelMath::bleChannelToNrfChannel(38));
    TEST_ASSERT_EQUAL_UINT8(80, ChannelMath::bleChannelToNrfChannel(39));

    // ZigBee Channels
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 2405.0f, ChannelMath::zigbeeChannelToFrequencyMHz(11));
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 2480.0f, ChannelMath::zigbeeChannelToFrequencyMHz(26));
}
