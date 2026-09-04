# Embedded Web Dashboard & API Guide

ESP32-RF-SWORD hosts a standalone, responsive cyber-themed Single Page Application directly from the ESP32's flash memory.

---

## Connecting to the Web Portal

1. **Connect to Wi-Fi**:
   - **SSID**: `SWORD-SECURITY-PORTAL`
   - **Password**: `rfsword123`
   - **Default IP**: `192.168.4.1`

2. **Captive Portal**:
   - When connecting on iOS, Android, macOS, or Windows, the captive portal prompt will automatically launch the dashboard.
   - Alternatively, navigate to `http://192.168.4.1` or `http://sword.local` in any modern web browser.

---

## Web Dashboard Features

```
+-------------------------------------------------------------------------+
| [⚔] ESP32-RF-SWORD [v2.5.0 PRO]              [● CONNECTED] [UP: 00:14:32] |
+-------------------------------------------------------------------------+
| [ ACTIVE MODE ]   [ HOP RATE ]    [ PACKET RATE ]   [ POWER ]   [ HEAP ] |
| SWEEP_COPRIME     6,520 hops/s    1,240 pkt/s       MAX (0dBm)  194 KB   |
+-----------------------------------+-------------------------------------+
| 2.4 GHz REAL-TIME WATERFALL       | CONTROL DECK                        |
|                                   | +---------------------------------+ |
|  [##############################] | |       [ STOP ATTACK ]           | |
|  [..............................] | +---------------------------------+ |
|  [2400MHz      2440MHz   2480MHz] | Mode: [ 2.4GHz Coprime Sweeper  ] |
|                                   | Preset: [ BLE Advertisement     ] |
| RADIO STATUS                      | Channel Range: Ch 2 -> Ch 80      |
| [R1] Ch 42 (2442 MHz) [=======--] | Dwell Jitter: 120 - 180 us        |
| [R2] Ch 81 (2481 MHz) [========-] | Power: [MIN] [LOW] [HIGH] [MAX]   |
+-----------------------------------+-------------------------------------+
| SYSTEM AUDIT LOG                                                        |
| [14:32:01] [OK][WS] WebSocket connected. Telemetry stream active.       |
+-------------------------------------------------------------------------+
```

1. **Real-Time Waterfall & Spectrum Chart**:
   - 60 FPS HTML5 Canvas waterfall rendering showing 128 channels simultaneously.
   - Toggle between 2D spectrum line graph and waterfall heatmap.
2. **Dynamic Sliders & Controls**:
   - Instantly adjust channel bounds, hop dwell times, output power, and attack presets without rebooting.
3. **Multi-Radio HUD**:
   - Real-time frequency gauges for up to 4 nRF24 radios and Sub-GHz CC1101 module.
4. **NVS Flash Synchronization**:
   - Click **Save to Flash** to persist current configuration across power cycles.

---

## WebSocket Protocol Specification (`/ws`)

The WebSocket interface streams JSON telemetry frames at 10 Hz and receives real-time JSON command messages.

### Incoming Command Message Format:
```json
{
  "action": "set_mode",
  "mode": "SWEEP_COPRIME"
}
```

#### Available Actions:
- `toggle_run`: `{"action": "toggle_run", "run": true|false}`
- `set_mode`: `{"action": "set_mode", "mode": "<MODE_NAME>"}`
- `set_preset`: `{"action": "set_preset", "preset": "<PRESET_NAME>"}`
- `set_channels`: `{"action": "set_channels", "min": 2, "max": 80}`
- `set_dwell`: `{"action": "set_dwell", "min": 120, "max": 180}`
- `set_power`: `{"action": "set_power", "power": "MIN|LOW|HIGH|MAX"}`
- `save_config`: `{"action": "save_config"}`
- `reboot`: `{"action": "reboot"}`

### Outgoing Telemetry Message Format:
```json
{
  "type": "telemetry",
  "telemetry": {
    "uptime": 142,
    "freeHeap": 194560,
    "tempC": 42.5,
    "hopsPerSec": 6520,
    "packetsPerSec": 1240,
    "mode": "SWEEP_COPRIME",
    "preset": "Full Band 2.4GHz",
    "power": "MAX (0 dBm / +20dBm PA)",
    "radioCount": 2,
    "running": true
  },
  "radios": [
    {"present": true, "active": true, "channel": 42, "freq": 2442.0},
    {"present": true, "active": true, "channel": 81, "freq": 2481.0}
  ],
  "spectrum": [0, 0, 45, 80, 100, 20, 0, ...]
}
```
