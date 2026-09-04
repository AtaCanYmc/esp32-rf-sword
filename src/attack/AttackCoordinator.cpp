/**
 * @file AttackCoordinator.cpp
 * @brief Master Attack Coordinator implementation
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#include "attack/AttackCoordinator.h"
#include "attack/SweepAttack.h"
#include "attack/TargetedAttack.h"
#include "attack/NoiseBurstAttack.h"
#include "attack/WiFiDeauthAttack.h"
#include "attack/BLESpamAttack.h"
#include "attack/SubGhzAttack.h"
#include "analyzer/SpectrumScanner.h"
#include "radio/NRF24Controller.h"
#include "core/SystemState.h"
#include "core/Logger.h"

#ifndef HOST_SIMULATION
#include <Arduino.h>
#endif

AttackCoordinator& AttackCoordinator::instance() {
    static AttackCoordinator s_instance;
    return s_instance;
}

AttackCoordinator::AttackCoordinator() {
}

void AttackCoordinator::init() {
    DeviceConfig cfg = SystemState::instance().getConfig();
    currentMode = cfg.defaultMode;
    currentPreset = cfg.defaultPreset;
    applyPreset(currentPreset);
    setMode(currentMode);
}

void AttackCoordinator::setMode(AttackMode mode) {
    if (currentMode != mode) {
        stop();
        currentMode = mode;
        modeInitialized = false;
        SystemState::instance().setMode(mode);
        SWORD_LOGI("ATTACK", "Mode switched to: %s", SystemState::instance().getModeName(mode));
    }
}

void AttackCoordinator::applyPreset(TargetPreset preset) {
    currentPreset = preset;
    SystemState::instance().setPreset(preset);
    const TargetProfile& prof = Presets::getProfile(preset);

    if (prof.subGhzFreqMHz > 0.0f) {
        SystemState::instance().setSubGhzParams(prof.subGhzFreqMHz, SubGhzModulation::MOD_CW, 10);
        setMode(AttackMode::TARGETED_SUBGHZ);
    } else {
        SystemState::instance().setChannels(prof.minChannel, prof.maxChannel);
        SystemState::instance().setDwellRangeUs(prof.minDwellUs, prof.maxDwellUs);
    }

    SWORD_LOGI("ATTACK", "Applied preset: %s [%s]", prof.name, prof.description);
}

void AttackCoordinator::stop() {
    NRF24Controller::instance().stopAll();
    SystemState::instance().stop();
    modeInitialized = false;
}

void AttackCoordinator::step() {
    if (!SystemState::instance().isRunning()) {
#ifndef HOST_SIMULATION
        delay(50);
#endif
        return;
    }

    // Initialize mode once on transition
    if (!modeInitialized) {
        switch (currentMode) {
            case AttackMode::SWEEP_COPRIME:
            case AttackMode::SWEEP_LINEAR:
            case AttackMode::SWEEP_RANDOM:
                SweepAttack::instance().init();
                break;
            case AttackMode::TARGETED_BLE:
            case AttackMode::TARGETED_WIFI:
            case AttackMode::TARGETED_ZIGBEE:
            case AttackMode::TARGETED_DRONE:
                TargetedAttack::instance().init(currentPreset);
                break;
            case AttackMode::NOISE_BURST:
                NoiseBurstAttack::instance().init();
                break;
            case AttackMode::SPECTRUM_ANALYZER:
                SpectrumScanner::instance().init();
                break;
            case AttackMode::WIFI_BEACON_SPAM:
            case AttackMode::WIFI_DEAUTH_STORM:
                WiFiDeauthAttack::instance().init();
                break;
            case AttackMode::BLE_POPUP_SPAM:
                BLESpamAttack::instance().init();
                break;
            case AttackMode::TARGETED_SUBGHZ: {
                float freq;
                SubGhzModulation mod;
                int8_t pwr;
                SystemState::instance().getSubGhzParams(freq, mod, pwr);
                SubGhzAttack::instance().init(freq, mod);
                break;
            }
            case AttackMode::TOTAL_BLITZ:
                SweepAttack::instance().init();
                WiFiDeauthAttack::instance().init();
                BLESpamAttack::instance().init();
                break;
            default:
                break;
        }
        modeInitialized = true;
    }

    // Dispatch step action according to active mode
    switch (currentMode) {
        case AttackMode::SWEEP_COPRIME:
            SweepAttack::instance().stepCoprimeSweep();
            break;
        case AttackMode::SWEEP_LINEAR:
            SweepAttack::instance().stepLinearSweep();
            break;
        case AttackMode::SWEEP_RANDOM:
            SweepAttack::instance().stepRandomSweep();
            break;
        case AttackMode::TARGETED_BLE:
        case AttackMode::TARGETED_WIFI:
        case AttackMode::TARGETED_ZIGBEE:
        case AttackMode::TARGETED_DRONE:
            TargetedAttack::instance().stepTargetedJammer();
            break;
        case AttackMode::NOISE_BURST:
            NoiseBurstAttack::instance().stepNoiseBurst();
            break;
        case AttackMode::SPECTRUM_ANALYZER:
            SpectrumScanner::instance().stepScan();
            break;
        case AttackMode::WIFI_BEACON_SPAM:
            WiFiDeauthAttack::instance().stepBeaconFlood();
            break;
        case AttackMode::WIFI_DEAUTH_STORM:
            WiFiDeauthAttack::instance().stepDeauthStorm();
            break;
        case AttackMode::BLE_POPUP_SPAM:
            BLESpamAttack::instance().stepBLESpam();
            break;
        case AttackMode::TARGETED_SUBGHZ:
            SubGhzAttack::instance().stepSubGhz();
            break;
        case AttackMode::TOTAL_BLITZ:
            executeTotalBlitz();
            break;
        case AttackMode::IDLE:
        default:
#ifndef HOST_SIMULATION
            delay(100);
#endif
            break;
    }
}

void AttackCoordinator::executeTotalBlitz() {
    // 1. Step nRF24 Coprime sweep
    SweepAttack::instance().stepCoprimeSweep();
    // 2. Step 802.11 Deauth frame
    WiFiDeauthAttack::instance().stepDeauthStorm();
    // 3. Step BLE popup spam
    BLESpamAttack::instance().stepBLESpam();
    // 4. Step Sub-GHz if available
    SubGhzAttack::instance().stepSubGhz();
}

AttackMode AttackCoordinator::getCurrentMode() const {
    return currentMode;
}

TargetPreset AttackCoordinator::getCurrentPreset() const {
    return currentPreset;
}
