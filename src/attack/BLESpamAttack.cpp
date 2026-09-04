/**
 * @file BLESpamAttack.cpp
 * @brief Native BLE Spammer Coordinator implementation
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#include "attack/BLESpamAttack.h"
#include "radio/BLEEngine.h"

#ifndef HOST_SIMULATION
#include <Arduino.h>
#endif

BLESpamAttack& BLESpamAttack::instance() {
    static BLESpamAttack s_instance;
    return s_instance;
}

BLESpamAttack::BLESpamAttack() {
}

void BLESpamAttack::init() {
    BLEEngine::instance().init();
}

void BLESpamAttack::stepBLESpam() {
    BLEEngine::instance().stepSpamCycle();
#ifndef HOST_SIMULATION
    delay(20);
#endif
}
