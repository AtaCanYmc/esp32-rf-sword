/**
 * @file test_presets.cpp
 * @brief Unit tests for Presets and Profile manager
 */

#include <unity.h>
#include "attack/Presets.h"
#include <string.h>

void test_preset_lookup() {
    TargetPreset p1 = Presets::getPresetByName("ble-adv");
    TEST_ASSERT_EQUAL_UINT8((uint8_t)TargetPreset::BLE_ADVERTISEMENT, (uint8_t)p1);

    TargetPreset p2 = Presets::getPresetByName("wifi-6");
    TEST_ASSERT_EQUAL_UINT8((uint8_t)TargetPreset::WIFI_CHANNEL_6, (uint8_t)p2);

    TargetPreset p3 = Presets::getPresetByName("sub-433");
    TEST_ASSERT_EQUAL_UINT8((uint8_t)TargetPreset::SUBGHZ_433MHZ, (uint8_t)p3);
}

void test_preset_profiles() {
    const TargetProfile& bleProf = Presets::getProfile(TargetPreset::BLE_ADVERTISEMENT);
    TEST_ASSERT_EQUAL_STRING("ble-adv", bleProf.name);
    TEST_ASSERT_EQUAL_UINT8(3, bleProf.channelListLen);
    TEST_ASSERT_EQUAL_UINT8(2, bleProf.channelList[0]);
    TEST_ASSERT_EQUAL_UINT8(26, bleProf.channelList[1]);
    TEST_ASSERT_EQUAL_UINT8(80, bleProf.channelList[2]);

    const TargetProfile& wifi1Prof = Presets::getProfile(TargetPreset::WIFI_CHANNEL_1);
    TEST_ASSERT_EQUAL_UINT8(1, wifi1Prof.minChannel);
    TEST_ASSERT_EQUAL_UINT8(23, wifi1Prof.maxChannel);

    const TargetProfile& subProf = Presets::getProfile(TargetPreset::SUBGHZ_433MHZ);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 433.92f, subProf.subGhzFreqMHz);
}
