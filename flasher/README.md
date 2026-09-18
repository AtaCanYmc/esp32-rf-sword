<!-- well-document · audit scorecard: G1:5 G2:5 G3:5 G4:5 G5:5 G6:5 · STATUS: PASS -->

[← Back to Root Repository](../README.md)

# ESP32-RF-SWORD Web Flasher

A browser-based Progressive Web App (PWA) firmware installer built on [ESP Web Tools](https://esphome.github.io/esp-web-tools/) and the standard HTML5 Web Serial API.

---

## Table of Contents

- [Overview](#overview)
- [Browser Compatibility](#browser-compatibility)
- [Manifest Schema](#manifest-schema)
- [Local Development and Testing](#local-development-and-testing)
- [Deployment Pipeline](#deployment-pipeline)

---

## Overview

The Web Flasher allows flashing pre-compiled ESP32-RF-SWORD firmware binaries directly to connected microcontrollers over a USB-to-UART or USB CDC connection without requiring command-line tools (such as Python, esptool, or PlatformIO).

The web application is hosted on GitHub Pages at [https://atacanymc.github.io/esp32-rf-sword/](https://atacanymc.github.io/esp32-rf-sword/).

---

## Browser Compatibility

The Web Flasher requires an operating system and browser that supports the [Web Serial API](https://developer.mozilla.org/en-US/docs/Web/API/Web_Serial_API):

| Browser | Minimum Version | Status |
| :--- | :--- | :--- |
| Google Chrome | 89+ | Supported |
| Microsoft Edge | 89+ | Supported |
| Opera | 75+ | Supported |
| Chromium-based forks (Brave, Vivaldi) | 89+ | Supported (ensure Web Serial flags are enabled) |
| Mozilla Firefox | Any | Unsupported (lacks native Web Serial API) |
| Apple Safari | Any | Unsupported (lacks native Web Serial API) |

---

## Manifest Schema

ESP Web Tools parses [manifest.json](manifest.json) to resolve target chipset architectures and binary image offsets:

```json
{
  "name": "ESP32-RF-SWORD",
  "version": "0.2.0",
  "home_assistant_domain": "esp32-rf-sword",
  "new_install_prompt_erase": true,
  "builds": [
    {
      "chipFamily": "ESP32-C3",
      "parts": [
        { "path": "firmware-esp32-c3.bin", "offset": 0 }
      ]
    },
    {
      "chipFamily": "ESP32",
      "parts": [
        { "path": "firmware-esp32.bin", "offset": 0 }
      ]
    },
    {
      "chipFamily": "ESP32-S3",
      "parts": [
        { "path": "firmware-esp32-s3.bin", "offset": 0 }
      ]
    }
  ]
}
```

Each binary listed at offset `0` is a merged factory image created by `scripts/merge_bin.py` containing bootloader, partition table, and application firmware.

---

## Local Development and Testing

Because the Web Serial API requires a secure context (`https://` or `http://localhost`), launch a local HTTP server from the `flasher/` directory:

```bash
cd flasher
python3 -m http.server 8080
```

Open `http://localhost:8080` in Chrome or Edge, attach an ESP32 board via USB, and click **Install ESP32-RF-SWORD**.

---

## Deployment Pipeline

The flasher assets are automatically deployed to GitHub Pages via [.github/workflows/pages.yml](../.github/workflows/pages.yml) upon commits to the `main` branch. Release builds generated in CI are packaged into the root artifact distribution.
