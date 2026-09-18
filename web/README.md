<!-- well-document · audit scorecard: G1:5 G2:5 G3:5 G4:5 G5:5 G6:5 · STATUS: PASS -->

[← Back to Root Repository](../README.md)

# Embedded Web Dashboard (`web/`)

This directory contains the front-end source code for the ESP32-RF-SWORD embedded control deck and real-time spectrum waterfall visualizer.

---

## Table of Contents

- [Overview](#overview)
- [Architecture & Design Constraints](#architecture--design-constraints)
- [Asset Files](#asset-files)
- [Bundling Pipeline](#bundling-pipeline)
- [WebSocket Telemetry Protocol](#websocket-telemetry-protocol)
- [Local Development and Mock Testing](#local-development-and-mock-testing)

---

## Overview

The web dashboard is served directly by the microcontroller via `ESPAsyncWebServer` from non-volatile flash memory. When a client connects to the `SWORD-SECURITY-PORTAL` Wi-Fi Access Point, an internal DNS server captures all requests on port 53 and redirects browser traffic to `http://192.168.4.1`.

---

## Architecture & Design Constraints

1. **Zero External Dependencies**:
   - The interface requires no external CDNs, fonts, or third-party JavaScript libraries. It operates entirely offline in isolated RF test environments.
2. **Deterministic Memory Footprint**:
   - The entire front-end (HTML, CSS, and JS) is compressed into a single compilation unit under 35 KB to minimize flash overhead.
3. **Hardware-Accelerated Rendering**:
   - The 128-channel spectrum waterfall is rendered using an HTML5 Canvas element via `requestAnimationFrame` at up to 60 frames per second without burdening the microcontroller CPU.

---

## Asset Files

| File | Purpose |
| :--- | :--- |
| `index.html` | Semantic markup for telemetry gauges, frequency sliders, attack mode selectors, and the waterfall canvas container. |
| `style.css` | Cyberpunk-themed dark UI styling, responsive layout grid, status indicators, and slider styling. |
| `app.js` | WebSocket lifecycle manager, telemetry JSON parser, canvas waterfall buffer renderer, and REST dispatchers. |

---

## Bundling Pipeline

The microcontroller firmware does not serve assets from SPIFFS or LittleFS directly. Instead, `scripts/bundle_web.py` inlines `style.css` and `app.js` into `index.html` and generates a C++ header containing a raw string literal:

```bash
# Rebundle assets into include/ui/WebAssets.h
python3 scripts/bundle_web.py
```

The continuous integration pipeline strictly enforces that `include/ui/WebAssets.h` remains synchronized with modifications in `web/`:

```bash
git diff --exit-code include/ui/WebAssets.h
```

---

## WebSocket Telemetry Protocol

The dashboard maintains a persistent bidirectional WebSocket connection at `ws://192.168.4.1/ws`.

### Inbound Telemetry (Microcontroller → Browser @ 10 Hz)

```json
{
  "running": true,
  "mode": 1,
  "preset": 0,
  "hopRate": 6480,
  "pktRate": 1220,
  "temp": 48.2,
  "heap": 198420,
  "uptime": 45,
  "radios": [
    { "id": 1, "active": true, "channel": 42, "freq": 2442.0 },
    { "id": 2, "active": true, "channel": 81, "freq": 2481.0 }
  ],
  "waterfall": [12, 14, 55, 89, 23, 10, 8, 4]
}
```

### Outbound Control (Browser → Microcontroller)

```json
{
  "cmd": "set_mode",
  "mode": "SWEEP_COPRIME",
  "preset": "BLE_ADV",
  "power": "MAX"
}
```

---

## Local Development and Mock Testing

To inspect and test front-end visual changes in a desktop browser without flashing a microcontroller:

```bash
cd web
python3 -m http.server 8080
```

Open `http://localhost:8080` in your web browser. Note that WebSocket telemetry updates require either an active ESP32 connection or a mock WebSocket server.
