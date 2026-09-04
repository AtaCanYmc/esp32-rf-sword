/**
 * @file BLESpamAttack.h
 * @brief Native BLE Spammer Coordinator
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#pragma once

#include "Types.h"
#include <stdint.h>

class BLESpamAttack {
public:
    static BLESpamAttack& instance();

    void init();
    void stepBLESpam();

private:
    BLESpamAttack();
    ~BLESpamAttack() = default;
};
