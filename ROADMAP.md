<!-- well-document · audit scorecard: G1:5 G2:5 G3:5 G4:5 G5:5 G6:5 · STATUS: PASS -->

# Project Roadmap

This document outlines completed milestones, current deliverables, and planned technical targets for ESP32-RF-SWORD. Items are organized by semantic version milestones and grounded in physical microcontroller and RF hardware capabilities.

---

## Completed Milestones

### Version 0.1.0 (Initial Architectural Release)
- [x] **Modular C++ Architecture**: Refactored monolithic prototype into layered abstraction boundaries (`hal`, `radio`, `attack`, `analyzer`, `ui`, `core`).
- [x] **Multi-Transceiver SPI Abstraction**: Implemented concurrent control for dual and quad nRF24L01+ transceivers with pre-conditioning against MISO line contention.
- [x] **Sub-GHz Transceiver Integration**: Embedded TI CC1101 driver for 315 MHz, 433.92 MHz, 868.35 MHz, and 915 MHz ISM bands.
- [x] **FreeRTOS Dual-Core Concurrency**: Pinned microsecond-accurate radio hopping to Core 1 (Priority 24) and isolated network/UI processing on Core 0.
- [x] **Control Interfaces**: Deployed embedded Captive Portal Web UI, 10 Hz WebSocket telemetry, interactive VT100 serial CLI, and SSD1306 OLED HUD.
- [x] **Spectrum Analysis**: Implemented 128-channel 2.4 GHz RSSI scanner with circular waterfall buffer.
- [x] **Host Unit Test Suite**: Developed host-side Clang test harness for channel permutation math, presets, and buffer lifecycle.

### Version 0.2.0 (Automation and Multi-Board Platform)
- [x] **Continuous Integration Pipeline**: Automated host unit tests, web bundle synchronisation checks, and PlatformIO builds across four board targets.
- [x] **Automated Release Management**: Configured Google Release-Please for changelog generation and semantic tagging from Conventional Commits.
- [x] **Automated Dependency Updates**: Enabled GitHub Dependabot for GitHub Actions and build tooling.
- [x] **Web Serial Installer**: Published progressive web flasher on GitHub Pages for single-click browser installation without toolchain setup.
- [x] **ESP32-C6 Platform Support**: Added board definition and pin mapping for the ESP32-C6 RISC-V platform.

---

## Active & Upcoming Milestones

### Version 0.3.0 (Protocol Expansion & RF Refinement)
- [ ] **Native IEEE 802.15.4 / Thread Engine (ESP32-C6)**:
  - Implement raw 802.15.4 beacon and data frame injection using ESP32-C6 native hardware radio.
  - Add ZigBee coordinator link verification profile.
- [ ] **Dynamic SPI Frequency Negotiation**:
  - Add runtime SPI bus speed benchmarking to dynamically select optimal clock rates (10 to 20 MHz) based on trace capacitance and wiring signal integrity.
- [ ] **Automated Power Attenuation Calibration**:
  - Store calibrated PA output power tables per band in NVS to ensure linear step adjustment across nRF24 and CC1101 modules.
- [ ] **Sub-GHz Custom Frequency Preset Profile**:
  - Expose CLI and Web controls for user-defined Sub-GHz frequency channels beyond default ISM allocations.

### Version 1.0.0 (Production Hardening)
- [ ] **HAL Stability & API Freeze**:
  - Freeze public interfaces in `include/hal/` and `include/radio/` for external firmware integration.
- [ ] **Brownout & Thermal Management**:
  - Implement dynamic core frequency scaling and automatic radio power throttling upon detection of supply rail voltage sag during simultaneous multi-radio transmission.
- [ ] **Automated Hardware-in-the-Loop (HIL) Test Harness**:
  - Integrate hardware-in-the-loop CI runners with attached SDR receivers (RTL-SDR / HackRF) to verify real RF emission spectra on pull requests.
