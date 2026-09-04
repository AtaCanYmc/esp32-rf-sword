/**
 * @file Config.h
 * @brief Global Configuration & Compilation Flags for ESP32-RF-SWORD
 * @author ESP32-RF-SWORD Core Team
 * @license MIT
 */

#pragma once

#include <stdint.h>

#ifndef SWORD_VERSION
#define SWORD_VERSION "2.5.0"
#endif

#define SWORD_PROJECT_NAME "ESP32-RF-SWORD"
#define SWORD_PROJECT_TAGLINE "Multi-Band RF Security & Research Toolkit"

// ==============================================================================
// Hardware & Bus Configuration Defaults
// ==============================================================================

#define DEFAULT_SPI_SPEED_HZ        16000000UL  // 16 MHz reliable SPI clock (supports up to 20MHz)
#define FAST_SPI_SPEED_HZ           20000000UL  // 20 MHz experimental turbo SPI
#define SAFE_SPI_SPEED_HZ           10000000UL  // 10 MHz fallback for long wires

#define SERIAL_BAUD_RATE            115200

// Maximum supported radios
#define MAX_NRF24_MODULES           4
#define MAX_SUBGHZ_MODULES          1

// RF Channels (2.4 GHz ISM: 2400 MHz + channel)
#define NRF24_MIN_CHANNEL           0
#define NRF24_MAX_CHANNEL           125
#define DEFAULT_SWEEP_MIN_CH        2           // 2402 MHz
#define DEFAULT_SWEEP_MAX_CH        80          // 2480 MHz (Wi-Fi + BLE full spectrum)
#define DEFAULT_COPRIME_STEP        37          // Coprime with 79 for uniform permutation
#define DEFAULT_RADIO_OFFSET        39          // Interleaved radio offset

// Dwell Times in Microseconds
#define DEFAULT_MIN_DWELL_US        120
#define DEFAULT_MAX_DWELL_US        180
#define ABSOLUTE_MIN_DWELL_US       10
#define ABSOLUTE_MAX_DWELL_US       10000

// ==============================================================================
// Wi-Fi & Web Portal Defaults
// ==============================================================================

#define DEFAULT_AP_SSID             "SWORD-SECURITY-PORTAL"
#define DEFAULT_AP_PASSWORD         "rfsword123"
#define DEFAULT_AP_CHANNEL          1
#define DEFAULT_HTTP_PORT           80
#define DEFAULT_WS_PORT             80

#define CAPTIVE_PORTAL_DNS_PORT     53

// ==============================================================================
// FreeRTOS Task Priorities & Stack Sizes
// ==============================================================================

#define RF_WORKER_CORE              1
#define RF_WORKER_PRIORITY          24          // Near-highest priority for tight RF timing
#define RF_WORKER_STACK_SIZE        8192

#define SYSTEM_MGR_CORE             0
#define SYSTEM_MGR_PRIORITY         5
#define SYSTEM_MGR_STACK_SIZE       8192

#define CLI_TASK_PRIORITY           3
#define CLI_TASK_STACK_SIZE         4096

#define DISPLAY_TASK_PRIORITY       2
#define DISPLAY_TASK_STACK_SIZE     4096

// ==============================================================================
// Safety Features
// ==============================================================================

#define WATCHDOG_TIMEOUT_SEC        10
#define DEFAULT_AUTO_STOP_TIMEOUT_S 0           // 0 = continuous (no timeout), >0 = auto-stop after N sec
#define MAX_TEMP_THROTTLE_CELSIUS   80.0f       // ESP32 internal temp sensor throttle threshold
