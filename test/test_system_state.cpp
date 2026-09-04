/**
 * @file test_system_state.cpp
 * @brief Unit tests for SystemState logic
 */

#include "unity.h"
#include "core/SystemState.h"

void test_system_state_controls() {
    SystemState& state = SystemState::instance();
    
    state.setMode(AttackMode::SWEEP_COPRIME);
    TEST_ASSERT_EQUAL_UINT8((uint8_t)AttackMode::SWEEP_COPRIME, (uint8_t)state.getMode());

    state.setPowerLevel(RadioPowerLevel::POWER_MAX);
    TEST_ASSERT_EQUAL_UINT8((uint8_t)RadioPowerLevel::POWER_MAX, (uint8_t)state.getPowerLevel());

    state.setChannels(5, 75);
    uint8_t minCh, maxCh;
    state.getChannels(minCh, maxCh);
    TEST_ASSERT_EQUAL_UINT8(5, minCh);
    TEST_ASSERT_EQUAL_UINT8(75, maxCh);

    state.setDwellRangeUs(100, 200);
    uint16_t minD, maxD;
    state.getDwellRangeUs(minD, maxD);
    TEST_ASSERT_EQUAL_UINT32(100, minD);
    TEST_ASSERT_EQUAL_UINT32(200, maxD);
}
