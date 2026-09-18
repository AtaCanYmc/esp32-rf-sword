<!-- well-document · audit scorecard: G1:5 G2:5 G3:5 G4:5 G5:5 G6:5 · STATUS: PASS -->

[← Back to Root Repository](../README.md)

# Embedded Web Dashboard and API Guide

ESP32-RF-SWORD hosts a standalone, responsive Single Page Application directly from the microcontroller flash memory.

---

## Connecting to the Web Portal

1. **Connect to Wi-Fi**:
   - **SSID**: `SWORD-SECURITY-PORTAL`
   - **Password**: `rfsword123`
   - **Default IP**: `192.168.4.1`

2. **Captive Portal**:
   - When connecting on iOS, Android, macOS, or Windows, the captive portal detection service launches the dashboard.
   - Alternatively, open `http://192.168.4.1` or `http://sword.local` in any modern web browser.

---

## Web Dashboard Architecture

```mermaid
flowchart TD
    subgraph WEB_DASHBOARD ["ESP32-RF-SWORD Web Portal"]
        subgraph HEADER ["Top Navigation and Status Bar"]
            BRAND["ESP32-RF-SWORD v0.2.0"]
            STATUS["WS Status: CONNECTED"]
            UPTIME["Uptime Counter"]
        end

        subgraph METRICS ["Real-Time Metric Cards"]
            M_MODE["Active Mode"]
            M_HOPS["Hop Rate (hops/s)"]
            M_PKT["Packet Rate (pkt/s)"]
            M_PWR["Power Level"]
            M_RAD["Radio Count"]
            M_HEALTH["Temperature and Free Heap"]
        end

        subgraph MAIN_BODY ["Live View and Control Deck"]
            subgraph LEFT_PANEL ["Spectrum Visualizer"]
                CANVAS["HTML5 Canvas Waterfall (Up to 60 FPS)"]
                GAUGES["Multi-Radio Frequency Gauges (R1 - R4)"]
            end
            subgraph RIGHT_PANEL ["Control Deck"]
                MASTER_BTN["Master Start / Stop Button"]
                MODE_SEL["Operation Mode Selector"]
                PRESET_SEL["Protocol Preset Selector"]
                CHAN_SLIDER["RF Channel Range Sliders (0 - 125)"]
                DWELL_SLIDER["Dwell Jitter Sliders (10 - 2000 µs)"]
                PWR_SEG["Power Selector (MIN / LOW / HIGH / MAX)"]
                FLASH_ACTIONS["Save to Flash and Reboot"]
            end
        end

        subgraph LOG_VIEW ["System Audit Console"]
            LOG_STREAM["Live WebSocket Terminal Log Stream"]
        end
    end
```

---

## WebSocket Lifecycle and Communication Flow

```mermaid
sequenceDiagram
    autonumber
    actor User as Researcher Browser
    participant Captive as Captive Portal / DNS
    participant WebSrv as AsyncWebServer (Port 80)
    participant WS as WebSocket (/ws @ 10 Hz)
    participant Core as SystemState & RF Coordinator

    User->>Captive: Connect to Wi-Fi SSID 'SWORD-SECURITY-PORTAL'
    Captive-->>User: Redirect to http://192.168.4.1/
    User->>WebSrv: HTTP GET /
    WebSrv-->>User: 200 OK (Inlined Web UI Bundle)
    User->>WS: WebSocket Connect (ws://192.168.4.1/ws)
    WS-->>User: WS Handshake Accepted

    loop Every 100ms (10 Hz Telemetry Stream)
        Core->>WS: Read Telemetry, Radios & 128-Ch Spectrum
        WS-->>User: JSON { type: "telemetry", telemetry: {...}, radios: [...], spectrum: [...] }
        User->>User: Render Canvas Waterfall & Gauges
    end

    opt User Triggered Action
        User->>WS: JSON { action: "set_mode", mode: "SWEEP_COPRIME" }
        WS->>Core: Update Mode & Reconfigure Radios
        Core-->>WS: Emit Audit Log
        WS-->>User: JSON { type: "log", msg: "Mode switched to SWEEP_COPRIME", level: "info" }
    end
```

---

## WebSocket Protocol Specification (`/ws`)

The WebSocket interface streams JSON telemetry frames at 10 Hz and receives real-time JSON command messages.

### Incoming Command Message Format

```json
{
  "action": "set_mode",
  "mode": "SWEEP_COPRIME"
}
```

#### Available Actions

| Action | Payload Example | Description |
| :--- | :--- | :--- |
| `toggle_run` | `{"action": "toggle_run", "run": true}` | Starts or stops RF transmission. |
| `set_mode` | `{"action": "set_mode", "mode": "SWEEP_COPRIME"}` | Sets active transmission mode. |
| `set_preset` | `{"action": "set_preset", "preset": "BLE_ADV"}` | Applies pre-defined protocol boundaries. |
| `set_channels`| `{"action": "set_channels", "min": 2, "max": 80}`| Sets custom channel bounds (0 to 125). |
| `set_dwell` | `{"action": "set_dwell", "min": 120, "max": 180}`| Sets Gaussian dwell jitter boundaries in µs. |
| `set_power` | `{"action": "set_power", "power": "MAX"}` | Sets RF power attenuation (MIN, LOW, HIGH, MAX). |
| `save_config` | `{"action": "save_config"}` | Flashes current settings to NVS storage. |
| `reboot` | `{"action": "reboot"}` | Reboots the microcontroller. |

### Outgoing Telemetry Message Format

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
  "spectrum": [0, 0, 45, 80, 100, 20, 0, 0]
}
```
