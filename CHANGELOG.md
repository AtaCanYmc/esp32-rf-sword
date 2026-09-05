# Changelog

All notable changes to the **ESP32-RF-SWORD** framework will be documented in this file.

---

## [0.2.0](https://github.com/AtaCanYmc/esp32-rf-sword/compare/v0.1.0...v0.2.0) (2026-09-05)


### Features

* **ci:** add Dependabot and Google Release-Please automation workflows ([2829f66](https://github.com/AtaCanYmc/esp32-rf-sword/commit/2829f663937d3ee40ec3b5f695c0f957defd7602))
* **docs:** add visual banner and fix repository CI badge URLs ([ebe32d7](https://github.com/AtaCanYmc/esp32-rf-sword/commit/ebe32d7b2e59720f948004653dafb339297cf35d))
* **flasher:** install favicons and web app manifest from RealFaviconGenerator ([58e901c](https://github.com/AtaCanYmc/esp32-rf-sword/commit/58e901cfd8ef11230c8ca4a0411b8c02a30789c7))


### Bug Fixes

* **build:** add pioarduino and dual-stack NimBLE support for ESP32-C6 ([01c769e](https://github.com/AtaCanYmc/esp32-rf-sword/commit/01c769e237ff4e885096eb3217fc5373ec02aa0e))


### Chores & Maintenance

* **deps:** apply dependabot updates and configure grouped PRs ([a5f21e5](https://github.com/AtaCanYmc/esp32-rf-sword/commit/a5f21e5edeadb88050698d7c7cbfa26c52158fd0))
* set initial release version to 0.1.0 ([9a17486](https://github.com/AtaCanYmc/esp32-rf-sword/commit/9a17486ac5de41bfe69e7001575edf090d7072a5))

## [0.1.0] - 2026-09-05 (Initial Release - Complete Re-Architecture from RF-KILL)

### Added
- **Multi-Board Profile Support**:
  - Out-of-the-box hardware profiles for `ESP32-C3 SuperMini`, `ESP32 DevKit V1 (WROOM-32)`, `ESP32-S3 DevKit`, and `ESP32-C6 DevKit`.
  - Dynamic pin remapping and NVS storage.
- **Multi-Radio Hardware Abstraction Layer (HAL)**:
  - Dual & Quad nRF24L01+ array management on shared SPI bus with pre-conditioning to prevent MISO contention.
  - Sub-GHz TI CC1101 module support for 315 MHz, 433.92 MHz, 868.35 MHz, and 915 MHz.
- **ESP32 Native Wi-Fi & BLE Attack Engines**:
  - Native 802.11 Beacon frame spamming (custom SSIDs, Rickroll lyrics, random generator).
  - Native 802.11 Deauthentication & Disassociation frame injector.
  - Native BLE advertisement flooder (Apple AirPods/AirTag popups, Android FastPair, Microsoft SwiftPair).
- **Advanced Attack & Research Modes**:
  - Coprime Sweeper, Linear Sweeper, Random Jitter Sweeper.
  - Focused Protocol Jammer (BLE Adv 37/38/39, Wi-Fi Ch 1/6/11, ZigBee 11-26, RC Drones).
  - High-Power Corrupted Packet Blaster (2Mbps CRC-disabled frames).
  - Total Blitz synchronized multi-band assault.
- **Spectrum Analysis & Waterfall Engine**:
  - Fast 128-channel 2.4 GHz RSSI energy scanner.
  - Circular waterfall buffer with 60 FPS HTML5 Canvas real-time visualizer.
  - Real-time ASCII spectrum graph in Serial terminal.
- **Interactive Control Interfaces**:
  - Cyberpunk-themed Single-Page Web Dashboard served from internal flash with captive portal.
  - 10 Hz WebSocket telemetry and bidirectional command stream.
  - Interactive VT100 / ANSI Serial CLI shell over USB CDC / UART at 115200 baud.
  - Optional I2C SSD1306 (128x64) OLED graphic HUD.
- **Dual-Core FreeRTOS Architecture**:
  - Dedicated Core 1 RF worker task (Priority 24) for microsecond-precise hopping.
  - Dedicated Core 0 System task for Web server, CLI, OLED, and NVS storage.
- **Comprehensive Test Suite**:
  - Unit tests for channel math, coprime distribution, presets, and circular buffer lifecycle.

### Changed (vs `pepeangell5/RF-KILL` Legacy)
- Translated entire project to English.
- Replaced hardcoded single-loop architecture with modular object-oriented C++ classes.
- Upgraded hardcoded channels to dynamic user-configurable spans, dwell timings, and presets.
- Added flash memory (NVS) persistence for all runtime settings.
