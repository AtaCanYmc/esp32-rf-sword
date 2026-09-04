# ESP32-RF-SWORD: System Architecture & Design

## 1. Executive Summary

ESP32-RF-SWORD is a modular, multi-radio RF transmission and spectrum research platform built on the ESP32 microcontroller family. It expands significantly upon basic 2.4 GHz sweep concepts by introducing multi-core FreeRTOS concurrency, multi-transceiver SPI multiplexing (Dual/Quad nRF24L01+ and Sub-GHz CC1101), native ESP32 802.11/BLE packet injection engines, an embedded real-time Web dashboard, and an ANSI interactive serial CLI.

---

## 2. Multi-Core Task Scheduling & Concurrency

```mermaid
flowchart TB
    subgraph ESP32_CHIP ["ESP32 DUAL-CORE HARDWARE ARCHITECTURE"]
        subgraph CORE_1 ["⚡ CORE 1: REAL-TIME RF WORKER (Priority 24 / Non-Blocking)"]
            RF_LOOP["Microsecond-Precise SPI Hopping Loop"]
            CW_GEN["Continuous Wave (CW) Carrier Generation"]
            COPRIME["Coprime Step Permutation Math"]
            JITTER["Gaussian Dwell Jitter Generator (10 - 5000 µs)"]
            NOISE_BLAST["High-Rate Corrupted Frame Blaster"]
            SPEC_SCAN["Fast 128-Channel RSSI Spectrum Scanner"]
        end

        subgraph CORE_0 ["🖥️ CORE 0: SYSTEM MANAGEMENT & NETWORK (Priority 5)"]
            WEB_SRV["AsyncWebServer (HTTP / REST API / Captive Portal)"]
            WS_SRV["AsyncWebSocket Telemetry Stream (10 Hz)"]
            SERIAL_CLI["Interactive ANSI Serial CLI (UART @ 115200)"]
            OLED_DRV["I2C SSD1306 Graphic Display Driver (4 Hz)"]
            NVS_SYNC["Flash NVS Preferences Storage Sync"]
            SAFETY["Hardware Watchdog & Thermal Throttle Check"]
        end

        subgraph SHARED_BUS ["THREAD-SAFE COMMUNICATION BUS"]
            MUTEX["FreeRTOS Mutex & Semaphores"]
            ATOMIC["Atomic State Flags & Metrics"]
        end
    end

    CORE_0 <==>|Non-blocking Thread-Safe Bus| SHARED_BUS
    CORE_1 <==>|Lock-Free Low-Latency Access| SHARED_BUS
```

### FreeRTOS Core Separation Benefits:
- **Zero RF Jitter**: The radio frequency hopping loop runs uninterrupted on Core 1 at priority 24 without being blocked by Wi-Fi TCP network handshakes, WebSocket serialization, or OLED I2C transactions.
- **Thread Safety**: Inter-core communication is managed via atomic variables, mutex locks (`SPIManager`, `Logger`, `SystemState`), and FreeRTOS semaphores.

---

## 3. High-Speed SPI Bus Multiplexing & CSN Conditioning

```mermaid
flowchart TD
    subgraph MCU ["ESP32 Microcontroller"]
        SPI_MASTER["Hardware SPI Host (FSPI / VSPI)<br/>Clock: 10 - 20 MHz"]
        GPIO_CSN1["GPIO CSN 1"]
        GPIO_CSN2["GPIO CSN 2"]
        GPIO_CSN3["GPIO CSN 3"]
        GPIO_CSN4["GPIO CSN 4"]
        GPIO_CSN_SUB["GPIO CSN Sub-GHz"]
    end

    subgraph SPI_LINES ["Shared High-Speed SPI Bus"]
        SCK["SCK (Clock)"]
        MOSI["MOSI (Master Out)"]
        MISO["MISO (Master In)"]
    end

    subgraph TRANSCEIVERS ["Multi-Transceiver Array"]
        R1["📡 Radio #1: nRF24L01+ (2.4 GHz)"]
        R2["📡 Radio #2: nRF24L01+ (2.4 GHz)"]
        R3["📡 Radio #3: nRF24L01+ (2.4 GHz)"]
        R4["📡 Radio #4: nRF24L01+ (2.4 GHz)"]
        RSUB["📻 Radio Sub-GHz: TI CC1101 (315-915 MHz)"]
    end

    SPI_MASTER --> SCK
    SPI_MASTER --> MOSI
    MISO --> SPI_MASTER

    SCK === R1 & R2 & R3 & R4 & RSUB
    MOSI === R1 & R2 & R3 & R4 & RSUB
    R1 & R2 & R3 & R4 & RSUB === MISO

    GPIO_CSN1 -->|Individual Select| R1
    GPIO_CSN2 -->|Individual Select| R2
    GPIO_CSN3 -->|Individual Select| R3
    GPIO_CSN4 -->|Individual Select| R4
    GPIO_CSN_SUB -->|Individual Select| RSUB
```

### Bus Conditioning Mechanism:
1. **Pre-Conditioning Phase**: Before initializing any transceiver, the firmware asserts all Chip Select Not (`CSN`) lines `HIGH` and all Chip Enable (`CE`) lines `LOW`. This forces all unselected radio transceivers into high-impedance (High-Z) mode on `MISO`, preventing SPI bus contention.
2. **Double Initialization**: A 500 ms power-rail settling delay is inserted between initial SPI initialization and configuration register writes, ensuring high-power PA/LNA front-ends are stable.
3. **Dynamic Clock Scaling**: The SPI bus defaults to 16 MHz with automatic support for 20 MHz experimental turbo mode or 10 MHz safe fallback.

---

## 4. Mathematical RF Hopping Algorithms

### Coprime Permutation Hop:
To cover an arbitrary span of $N$ RF channels uniformly without repeating until all channels are visited, the firmware calculates an optimal step $S$ such that:
$$\gcd(S, N) = 1$$

The recurrence relation for Radio 0 is:
$$\text{Channel}_0(k) = \text{minChannel} + (k \cdot S) \pmod N$$

For a multi-radio array of $M$ transceivers, each subsequent radio is offset by an orthogonal stride $\Delta = \lfloor N / M \rfloor$:
$$\text{Channel}_m(k) = \text{minChannel} + ((k \cdot S) + m \cdot \Delta) \pmod N$$

This mathematical guarantee ensures that no two radios ever transmit on the same channel simultaneously during the entire cycle.

### Gaussian Dwell Jitter:
To defeat spectrum counter analyzers and adaptive receiver notch filters, the dwell duration $t_{\text{dwell}}$ on each channel is varied pseudo-randomly:
$$t_{\text{dwell}} \sim \mathcal{U}(t_{\min}, t_{\max})$$
where $t_{\min} = 120\,\mu\text{s}$ and $t_{\max} = 180\,\mu\text{s}$ by default.

---

## 5. Software Layers

```mermaid
flowchart TD
    subgraph UI ["Layer 5: Presentation & User Interfaces (ui/)"]
        direction LR
        W["WebServerManager<br/>(AsyncWeb / WebSocket / Captive)"]
        CLI["SerialCLI<br/>(ANSI VT100 Shell)"]
        DISP["DisplayManager<br/>(I2C SSD1306 HUD)"]
    end

    subgraph ATTACK ["Layer 4: Attack & Analysis Engine (attack/ & analyzer/)"]
        direction LR
        COORD["AttackCoordinator"]
        SWEEP_MOD["SweepAttack / TargetedAttack / NoiseBurst"]
        SPEC_MOD["SpectrumScanner / WaterfallBuffer"]
    end

    subgraph RADIO ["Layer 3: Radio Transceiver Drivers (radio/)"]
        direction LR
        NRF_MOD["NRF24Controller (Dual/Quad 2.4GHz)"]
        CC_MOD["CC1101Controller (Sub-GHz)"]
        WIFI_MOD["WiFiEngine (802.11 Raw)"]
        BLE_MOD["BLEEngine (BLE 4.2/5.0)"]
    end

    subgraph CORE ["Layer 2: Core System & Concurrency (core/)"]
        direction LR
        STATE["SystemState"]
        NVS_MOD["NVSManager"]
        TASK_MOD["TaskManager"]
        LOG_MOD["Logger"]
    end

    subgraph HAL ["Layer 1: Hardware Abstraction (hal/)"]
        direction LR
        PINS["PinDefinitions & BoardProfiles"]
        SPI_MGR["SPIManager (Multiplex & Mutex)"]
    end

    UI --> CORE
    UI --> ATTACK
    ATTACK --> RADIO
    ATTACK --> CORE
    RADIO --> HAL
    CORE --> HAL
```

1. **Hardware Abstraction Layer (`hal/`)**: Board profile auto-detection (`ESP32-C3`, `ESP32-S3`, `ESP32 DevKit`, `ESP32-C6`), GPIO pin mappings, SPI bus arbitration.
2. **Radio Engine Layer (`radio/`)**: Low-level register drivers for nRF24L01+, TI CC1101, native ESP32 Wi-Fi raw frame engine, and native BLE advertising engine.
3. **Attack & Research Layer (`attack/`)**: High-level attack coordinators, sweepers, protocol jammers, noise generators, and targeted frequency profiles.
4. **Spectrum Analysis Layer (`analyzer/`)**: Fast 128-channel RSSI scanner and circular waterfall buffer.
5. **User Interface Layer (`ui/`)**: Async Web server, WebSocket telemetry broadcaster, ANSI VT100 interactive Serial CLI, and I2C SSD1306 graphic display driver.
6. **Core System Layer (`core/`)**: Atomic runtime state, NVS flash persistence, FreeRTOS task coordinator, and thread-safe color logger.
