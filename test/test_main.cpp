/**
 * @file test_main.cpp
 * @brief Comprehensive Test Runner entry point
 */

#include "unity.h"

// Forward declarations
void test_gcd();
void test_coprime_detection();
void test_coprime_permutation_completeness();
void test_channel_frequency_conversions();
void test_preset_lookup();
void test_preset_profiles();
void test_waterfall_buffer_lifecycle();
void test_waterfall_overflow();
void test_system_state_controls();

int main(int argc, char** argv) {
    UNITY_BEGIN();

    // ChannelMath Tests
    RUN_TEST(test_gcd);
    RUN_TEST(test_coprime_detection);
    RUN_TEST(test_coprime_permutation_completeness);
    RUN_TEST(test_channel_frequency_conversions);

    // Preset Tests
    RUN_TEST(test_preset_lookup);
    RUN_TEST(test_preset_profiles);

    // Waterfall Buffer Tests
    RUN_TEST(test_waterfall_buffer_lifecycle);
    RUN_TEST(test_waterfall_overflow);

    // SystemState Tests
    RUN_TEST(test_system_state_controls);

    return UNITY_END();
}
