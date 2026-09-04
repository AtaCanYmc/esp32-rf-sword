```text
  ███████╗███████╗██████╗ ██████╗ ██████╗         ██████╗ ███████╗    ███████╗██╗    ██╗ ██████╗ ██████╗ ██████╗ 
  ██╔════╝██╔════╝██╔══██╗╚════██╗╚════██╗        ██╔══██╗██╔════╝    ██╔════╝██║    ██║██╔═══██╗██╔══██╗██╔══██╗
  █████╗  ███████╗██████╔╝ █████╔╝ █████╔╝█████╗  ██████╔╝█████╗      ███████╗██║ █╗ ██║██║   ██║██████╔╝██║  ██║
  ██╔══╝  ╚════██║██╔═══╝  ╚═══██╗██╔═══╝ ╚════╝  ██╔══██╗██╔══╝      ╚════██║██║███╗██║██║   ██║██╔══██╗██║  ██║
  ███████╗███████║██║     ██████╔╝███████╗        ██║  ██║██║         ███████║╚███╔███╔╝╚██████╔╝██║  ██║██████╔╝
  ╚══════╝╚══════╝╚═╝     ╚═════╝ ╚══════╝        ╚═╝  ╚═╝╚═╝         ╚══════╝ ╚══╝╚══╝  ╚═════╝ ╚═╝  ╚═╝╚═════╝ 
                      ⚔️ [ ADVANCED MULTI-BAND RF SECURITY & RESEARCH TOOLKIT ] ⚔️
```

# ESP32-RF-SWORD ⚔️

[![CI Build](https://github.com/your-username/esp32-rf-sword/actions/workflows/ci.yml/badge.svg)](https://github.com/your-username/esp32-rf-sword/actions/workflows/ci.yml)
[![Version](https://img.shields.io/badge/Version-2.5.0_PRO-00f0ff.svg?style=for-the-badge)](https://github.com/your-username/esp32-rf-sword)
[![Web Flasher](https://img.shields.io/badge/Web_Flasher-1--Click_Install-FF3366.svg?style=for-the-badge&logo=googlechrome)](flasher/index.html)
[![Framework](https://img.shields.io/badge/Framework-Arduino_%2F_ESP--IDF-00979D.svg?style=for-the-badge&logo=espressif)](https://espressif.com)
[![PlatformIO](https://img.shields.io/badge/PlatformIO-Supported-orange.svg?style=for-the-badge&logo=platformio)](https://platformio.org)
[![Hardware](https://img.shields.io/badge/Hardware-ESP32_%7C_C3_%7C_S3_%7C_C6-2F80ED.svg?style=for-the-badge)](docs/PINOUTS.md)
[![Radios](https://img.shields.io/badge/Radios-Dual%2FQuad_nRF24_%2B_CC1101_%2B_Native_Wi--Fi%2FBLE-FF9500.svg?style=for-the-badge)](docs/FREQUENCY_GUIDE.md)
[![License](https://img.shields.io/badge/License-MIT-35C759.svg?style=for-the-badge)](LICENSE)

> **ESP32-RF-SWORD** is an advanced, industrial-grade multi-band radio frequency (RF) security research, spectrum analysis, and signal transmission framework built for the ESP32 family (ESP32 Classic / ESP32-S3 / ESP32-C3 / ESP32-C6).
>
> It re-engineers, modernizes, and drastically expands the core concepts of [`pepeangell5/RF-KILL`](https://github.com/pepeangell5/RF-KILL) into a high-performance, modular C++20 architecture with multi-radio concurrency, embedded real-time Web dashboard, interactive ANSI CLI shell, and multi-band protocol engines.

---

## ⚡ Comparison: `RF-KILL` vs `ESP32-RF-SWORD`

| Feature | Legacy RF-KILL | ESP32-RF-SWORD (This Project) |
|---|---|---|
| **Language & Docs** | Spanish only (Minimal) | **Professional English (Comprehensive)** |
| **Code Architecture** | Monolithic single `.cpp` file | **Modular Object-Oriented C++ (HAL, Radio, UI, Core)** |
| **Board Support** | ESP32-C3 & Basic DevKit | **ESP32 Classic, ESP32-S3, ESP32-C3 SuperMini, ESP32-C6** |
| **Radio Support** | 2x nRF24 only | **Dual / Quad nRF24 (2.4GHz) + CC1101 (Sub-GHz) + Native ESP32** |
| **Concurrency** | Single blocking `loop()` | **FreeRTOS Dual-Core (Core 1: RF Worker @ Priority 24, Core 0: UI/Web)** |
| **User Interfaces** | Headless only | **Cyberpunk Web UI (Captive Portal), ANSI Serial CLI, I2C OLED HUD** |
| **Real-Time Telemetry**| Basic Serial prints | **10 Hz WebSockets + 60 FPS HTML5 Canvas Waterfall Spectrum** |
| **Spectrum Analysis** | None | **128-Channel 2.4 GHz RSSI Scanner & Waterfall Buffer** |
| **Native Wi-Fi Offense**| None | **Raw 802.11 Beacon Spammer, Deauth/Disassoc Injection** |
| **Native BLE Offense** | None | **Apple iOS Popups, Android FastPair & Windows SwiftPair Flooder** |
| **Sub-GHz Transceiver** | None | **TI CC1101 (315 / 433.92 / 868.35 / 915 MHz) CW & Sweep Jammer** |
| **Configuration** | Hardcoded macros | **Non-Volatile Flash Memory (NVS) Storage & Dynamic Remapping** |
| **Target Presets** | Fixed 79 channels | **BLE Adv (37/38/39), Wi-Fi (1/6/11), ZigBee (11-26), Drones, Custom** |
| **Automated Testing** | None | **Native Unit Test Suite for ChannelMath, Presets, and Buffers** |

---

## 🏛️ System Architecture

```mermaid
flowchart TD
    subgraph UI_LAYER ["User Interface & Control Layer"]
        WEB["🌐 SwordWeb UI<br/>(Captive Portal / HTTP / WebSocket)"]
        CLI["💻 Sword-CLI<br/>(ANSI Interactive Shell @ 115200)"]
        OLED["📺 OLED Display HUD<br/>(SSD1306 / I2C 128x64)"]
    end

    subgraph CORE_LAYER ["Core Management & Concurrency (Core 0)"]
        SYS["⚙️ SystemState<br/>(Atomic Metrics / Telemetry)"]
        NVS["💾 NVS Flash Manager<br/>(Preferences Persistence)"]
        TASK["🔄 TaskManager<br/>(FreeRTOS Dual-Core Scheduler)"]
    end

    subgraph ATTACK_LAYER ["RF Attack & Research Coordinator (Core 1 - Priority 24)"]
        COORD["⚔️ AttackCoordinator Master"]
        SWEEP["🌊 Coprime / Linear / Random Sweeper"]
        TARGET["🎯 Protocol Targeter (BLE/Wi-Fi/ZigBee/Drone)"]
        NOISE["⚡ Corrupted Frame / Noise Blaster"]
        SPEC["📊 128-Ch 2.4GHz Spectrum Scanner"]
    end

    subgraph RADIO_LAYER ["Hardware Transceivers & Native Engines"]
        SPI_BUS["⚡ High-Speed Shared SPI Bus (Up to 20MHz)"]
        NRF_ARR["📡 Dual / Quad nRF24L01+ Array<br/>(2.4 GHz ISM 2400-2525 MHz)"]
        CC1101["📻 TI CC1101 Sub-GHz Module<br/>(315 / 433 / 868 / 915 MHz)"]
        WIFI_ENG["📶 ESP32 Native 802.11 Engine<br/>(Raw Beacon Flood / Deauth Storm)"]
        BLE_ENG["📱 ESP32 Native BLE Engine<br/>(Apple / FastPair / SwiftPair Flood)"]
    end

    WEB <-->|WebSocket / REST| SYS
    CLI <-->|UART / USB CDC| SYS
    OLED <--|I2C| SYS

    SYS <--> CORE_LAYER
    TASK -->|Launches Priority 24 Task| COORD

    COORD --> SWEEP
    COORD --> TARGET
    COORD --> NOISE
    COORD --> SPEC

    SWEEP --> SPI_BUS
    TARGET --> SPI_BUS
    NOISE --> SPI_BUS
    SPEC --> SPI_BUS

    SPI_BUS --> NRF_ARR
    SPI_BUS --> CC1101
    COORD --> WIFI_ENG
    COORD --> BLE_ENG
```

---

## 🚀 Key Features & Attack Arsenal

### 1. Multi-Transceiver 2.4 GHz Sweeper Array
- **Dual & Quad nRF24L01+ Engine**: Simultaneous multi-radio multiplexing up to 20 MHz SPI bus clock.
- **Coprime Interleaved Sweep**: Mathematical coprime step calculation ($\gcd(S, N) = 1$) ensures complete, non-overlapping channel coverage across any custom frequency span.
- **Gaussian Jitter Dwell**: Prevents external spectrum counters from locking onto harmonic cycles.
- **Multi-Radio Quadrature Offsets**: Distributes transmissions across multiple radios without mutual interference.

### 2. Targeted Protocol Jamming & Interdiction
- **Bluetooth Low Energy (BLE)**: Pinpoint energy locking on Primary Advertising Channels (37 = 2402 MHz, 38 = 2426 MHz, 39 = 2480 MHz) or data hopping channels.
- **Wi-Fi 802.11 b/g/n Channel Blocking**: Shaped power blasting across Wi-Fi Channel 1 (2412 MHz), Channel 6 (2437 MHz), Channel 11 (2462 MHz), or custom 20/40 MHz bandwidths.
- **ZigBee / Thread / 802.15.4**: Channels 11 to 26 (2405 - 2480 MHz, 5 MHz spacing).
- **RC Drones & FPV Interdictor**: Targeted disruption of FlySky AFHDS, FrSky ACCST, and ExpressLRS links.

### 3. Sub-GHz Transceiver Support (TI CC1101)
- Supports **315 MHz**, **433.92 MHz**, **868.35 MHz**, and **915 MHz** ISM bands.
- Continuous Wave (CW) Carrier, OOK/FSK Modulated Noise, and Frequency Sweep Modes.

### 4. Native ESP32 Wi-Fi & BLE Offense Engine
- **802.11 Raw Frame Injector**:
  - Multi-SSID Beacon Flooder (custom lists, random names, Rickroll generator).
  - Deauthentication and Disassociation frame injection for 802.11 penetration auditing.
- **BLE Advertisement Flooder**:
  - Apple iOS AirDrop / AirPods popup storms.
  - Android Google FastPair flood.
  - Microsoft Windows SwiftPair flood.

### 5. 2.4 GHz Spectrum Analyzer & Waterfall
- Fast 128-channel RSSI scanner with real-time waterfall streaming to WebSockets, ANSI Serial terminal, and OLED screen.

---

## 🌐 Embedded Web Dashboard & Captive Portal

When powered on, ESP32-RF-SWORD broadcasts a standalone Wi-Fi Access Point:
- **SSID**: `SWORD-SECURITY-PORTAL`
- **Password**: `rfsword123`
- **URL**: `http://192.168.4.1`

Features:
- **60 FPS Real-Time Canvas Waterfall** showing RF energy across 128 channels.
- **Interactive Control Deck**: Start/Stop master button, Mode selector, Preset picker, Frequency sliders, Power toggles.
- **Live Telemetry Metrics**: Uptime, Core temperature, Free heap, Hop rate (hops/sec), Packet rate (pkt/sec).
- **Audit Log Console**: Real-time WebSocket event streaming.
- **NVS Flash Persistence**: Save parameters directly into non-volatile storage.

---

## 💻 Interactive Serial CLI Shell

Connect over USB CDC or UART at **115200 baud** for instant terminal access:

```text
  ______  _____ _____ ____ ___     ____  _____       ______          ______  ____  ____ 
 |  ____|/ ____|  __ \___ \__ \   |  _ \|  ___|     / _____|\      /|/  __  \|  _ \|  _ \
 | |__  | (___ | |__) |__) | ) |  | |_) | |_ _____  \_____ \ \ \/\ / /| |  | | |_) | | | |
 |  __|  \___ \|  ___/|__ < / /   |  _ <|  _/ ____|  _____) \ \V  V / | |  | |  _ <| |_| |
 | |____ ____) | |    ___) / /_   | |_) | | |       |______/ \_/\_/  | |__| | | \ \|____/ 
 |______|_____/|_|   |____/____|  |____/|_|                           \______/|_|  \_\     

 [ ESP32-RF-SWORD // Multi-Band RF Security & Research Toolkit v2.5.0 ]
 Type 'help' to see available commands or 'status' for real-time telemetry.

sword > status
=== SYSTEM TELEMETRY & STATUS ===
  State          : RUNNING
  Active Mode    : SWEEP_COPRIME
  Active Preset  : Full Band 2.4GHz (2402-2480 MHz)
  Hop Rate       : 6,480 hops/sec (Total: 48,120)
  Packet Rate    : 1,220 packets/sec (Total: 8,400)
  RF Power       : MAX (0 dBm / +20dBm PA)
  Channel Range  : Ch 2 (2402 MHz) -> Ch 80 (2480 MHz)
  Dwell Jitter   : 120 - 180 us
  Active Radios  : 2 connected
    Radio #1: Active=YES, Ch=42 (2442.0 MHz), TotalHops=24060
    Radio #2: Active=YES, Ch=81 (2481.0 MHz), TotalHops=24060
  Uptime / Heap  : 45 sec | Free Heap: 198420 bytes (Min: 182100)
```

### CLI Command Summary:
- `status`: Show live metrics and radio channels.
- `start` / `stop`: Toggle RF transmission.
- `mode <coprime|linear|random|ble|wifi|zigbee|drone|subghz|noise|spectrum|beacon|deauth|popup|blitz>`
- `preset <full|ble-adv|ble-all|wifi-1|wifi-6|wifi-11|zigbee|flysky|frsky|elrs|sub-315|sub-433|sub-868|sub-915>`
- `channels <min> <max>`: Set custom channel span.
- `dwell <min_us> <max_us>`: Set dwell jitter range.
- `power <min|low|high|max>`: Set output power.
- `spectrum`: Print live ASCII 2.4GHz spectrum bar graph.
- `pinout`: Print active hardware pin mapping.
- `save` / `reset` / `reboot`: Flash management & reboot.

---

## 🛠️ Hardware Wiring & Pinouts

### 1. ESP32-C3 SuperMini (Headless / Ultra-Compact)
| Signal | ESP32-C3 GPIO | Description |
|---|---|---|
| **SPI SCK** | `GPIO 4` | Shared SPI Clock |
| **SPI MISO** | `GPIO 5` | Shared SPI MISO |
| **SPI MOSI** | `GPIO 6` | Shared SPI MOSI |
| **Radio 1 CE / CSN** | `GPIO 3` / `GPIO 7` | nRF24 Radio #1 |
| **Radio 2 CE / CSN** | `GPIO 1` / `GPIO 10` | nRF24 Radio #2 |
| **CC1101 CSN / GDO0** | `GPIO 0` / `GPIO 2` | Sub-GHz Module |
| **I2C SDA / SCL** | `GPIO 8` / `GPIO 9` | OLED Display |

### 2. ESP32 DevKit V1 / WROOM-32 (Standard Dual-Core)
| Signal | ESP32 GPIO | Description |
|---|---|---|
| **SPI SCK / MISO / MOSI** | `GPIO 18` / `GPIO 19` / `GPIO 23` | Shared VSPI Bus |
| **Radio 1 CE / CSN** | `GPIO 4` / `GPIO 5` | nRF24 Radio #1 |
| **Radio 2 CE / CSN** | `GPIO 2` / `GPIO 15` | nRF24 Radio #2 |
| **Radio 3 CE / CSN** | `GPIO 13` / `GPIO 12` | nRF24 Radio #3 |
| **Radio 4 CE / CSN** | `GPIO 14` / `GPIO 27` | nRF24 Radio #4 |
| **CC1101 CSN / GDO0 / GDO2** | `GPIO 26` / `GPIO 25` / `GPIO 33` | Sub-GHz Module |
| **I2C SDA / SCL** | `GPIO 21` / `GPIO 22` | SSD1306 OLED (0x3C) |

*For ESP32-S3 and ESP32-C6 pinouts, see [`docs/PINOUTS.md`](docs/PINOUTS.md).*

---

## 📦 Building & Flashing

### Prerequisites:
- [PlatformIO Core](https://platformio.org/install/cli) or PlatformIO IDE extension for VS Code / CLion.
- Python 3.8+

### Step 1: Clone Repository
```bash
git clone https://github.com/your-username/esp32-rf-sword.git
cd esp32-rf-sword
```

### Step 2: Build & Flash for Your Board
- **For ESP32-C3 SuperMini**:
  ```bash
  pio run -e esp32-c3-supermini --target upload
  ```
- **For ESP32 DevKit V1 (WROOM-32)**:
  ```bash
  pio run -e esp32-devkit-v1 --target upload
  ```
- **For ESP32-S3 DevKit**:
  ```bash
  pio run -e esp32-s3-devkit --target upload
  ```

### Step 3: Open Serial Monitor
```bash
pio device monitor -b 115200
```

---

## 🧪 Automated Testing & Verification

Run the native test suite directly on your host machine:

```bash
clang++ -std=c++17 -DHOST_SIMULATION -Iinclude -Isrc -Itest \
  test/test_main.cpp test/test_channel_math.cpp test/test_presets.cpp \
  test/test_waterfall.cpp test/test_system_state.cpp \
  src/radio/ChannelMath.cpp src/attack/Presets.cpp \
  src/analyzer/WaterfallBuffer.cpp src/core/SystemState.cpp \
  -o /tmp/sword_test && /tmp/sword_test
```

---

## 📚 Technical Documentation Index

- [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md) - Deep dive into FreeRTOS core scheduling, SPI arbitration, and timing jitter physics.
- [`docs/PINOUTS.md`](docs/PINOUTS.md) - Complete wiring schematics, decoupling capacitor guides, and pin tables.
- [`docs/CLI_REFERENCE.md`](docs/CLI_REFERENCE.md) - Complete command reference for the interactive serial terminal.
- [`docs/WEB_INTERFACE.md`](docs/WEB_INTERFACE.md) - Web dashboard documentation, WebSocket protocol schema, and Captive Portal setup.
- [`docs/FREQUENCY_GUIDE.md`](docs/FREQUENCY_GUIDE.md) - Comprehensive RF frequency mapping for 2.4GHz ISM, Wi-Fi 1-14, BLE, ZigBee, and Sub-GHz.
- [`SECURITY.md`](SECURITY.md) - Ethical usage policy and responsible vulnerability disclosure.

---

## ⚖️ Legal & Ethical Notice

> [!CAUTION]
> **ESP32-RF-SWORD is developed strictly for academic research, authorized RF testing, laboratory hardware auditing, and educational demonstrations in controlled RF anechoic chambers or Faraday environments.**
> Transmitting on radio frequencies or disrupting wireless networks without proper legal authorization is strictly prohibited under national and international telecommunications law (e.g. FCC Title 47, Part 15, ITU Radio Regulations, ETSI RED).
> The authors accept no liability for damages or legal violations resulting from the use or misuse of this software.

---

## 📄 License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.
