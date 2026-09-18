<!-- well-document · audit scorecard: G1:5 G2:5 G3:5 G4:5 G5:5 G6:5 · STATUS: PASS -->

[← Back to Root Repository](../README.md)

# Technical Documentation Index

This directory contains technical specifications, hardware integration schematics, protocol definitions, and CLI command references for ESP32-RF-SWORD.

---

## Documents

| Document | Description |
| :--- | :--- |
| [ARCHITECTURE.md](ARCHITECTURE.md) | In-depth breakdown of the FreeRTOS dual-core concurrency model, SPI bus arbitration, MISO contention prevention, and mathematical Gaussian dwell jitter models. |
| [PINOUTS.md](PINOUTS.md) | Wiring diagrams and GPIO pinout mappings for ESP32 DevKit V1, ESP32-C3 SuperMini, ESP32-S3 DevKit, and ESP32-C6 DevKit, including decoupling capacitor guidelines. |
| [CLI_REFERENCE.md](CLI_REFERENCE.md) | Complete reference for the interactive VT100/ANSI serial terminal interface at 115200 baud, including syntax, flags, and script automation examples. |
| [WEB_INTERFACE.md](WEB_INTERFACE.md) | Specification for the embedded captive portal, RESTful configuration endpoints, and real-time WebSocket binary/JSON serialization schemas. |
| [FREQUENCY_GUIDE.md](FREQUENCY_GUIDE.md) | RF channel tables, bandwidth allocations, and carrier calculations across the 2.4 GHz ISM band (Wi-Fi 1-14, BLE 0-39, ZigBee 11-26) and Sub-GHz allocations (315, 433, 868, 915 MHz). |

---

## Contributing to Documentation

When updating or adding documentation:
- Keep explanations declarative, concise, and focused on technical specifications.
- Avoid informal language, marketing adjectives, and decorative inline emojis.
- Ensure all code blocks and command lines are copy-paste executable against the current codebase.
