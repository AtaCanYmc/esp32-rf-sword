# Hardware Wiring & Pinout Guide

This document contains complete wiring schematics and GPIO pinout mappings for all supported ESP32 boards.

---

## Power Supply & Decoupling Capacitor Notice (CRITICAL)

> [!IMPORTANT]
> **nRF24L01+ and CC1101 PA/LNA Modules require stable 3.3V power.**
> High-power PA+LNA modules draw current spikes up to 150–250 mA during RF transmission bursts.
> 1. Solder a **10 µF to 100 µF electrolytic or tantalum capacitor** (plus a 0.1 µF ceramic capacitor) directly across the `VCC` (3.3V) and `GND` pins on the bottom of each radio module.
> 2. **NEVER connect nRF24 or CC1101 to 5V power rails.** 5V will permanently destroy the radio transceivers. (However, their SPI digital input pins are 5V-tolerant).

---

## Profile 1: ESP32-C3 SuperMini (Headless / Ultra-Compact)

The ESP32-C3 SuperMini is an ultra-small single-core RISC-V board ideal for building pocket-sized 2.4 GHz multi-radio devices.

```
                    ESP32-C3 SuperMini Pinout Map
                         +-----------------+
                         | [USB Type-C]    |
                  GPIO 0 | 0             5V| 5V (USB In)
      Radio A CE  GPIO 3 | 3             GND| GND
     Radio A CSN  GPIO 7 | 7            3V3| 3.3V (VCC Out)
     Radio B CSN GPIO 10 | 10             9| GPIO 9 (Boot Button / I2C SCL)
      Radio B CE  GPIO 1 | 1              8| GPIO 8 (LED / I2C SDA)
        SPI SCK   GPIO 4 | 4              6| GPIO 6 (SPI MOSI)
        SPI MISO  GPIO 5 | 5              7| GPIO 7 (Radio A CSN)
                         +-----------------+
```

### Wiring Table:
| Component | Signal | ESP32-C3 GPIO | Description |
|---|---|---|---|
| **SPI Bus** | SCK | `GPIO 4` | Shared SPI Clock |
| | MISO | `GPIO 5` | Shared SPI Master-In-Slave-Out |
| | MOSI | `GPIO 6` | Shared SPI Master-Out-Slave-In |
| | VCC | `3.3V` | 3.3V Power (with capacitor) |
| | GND | `GND` | Common Ground |
| **nRF24 #1 (Radio A)** | CE | `GPIO 3` | Radio A Chip Enable |
| | CSN | `GPIO 7` | Radio A Chip Select |
| **nRF24 #2 (Radio B)** | CE | `GPIO 1` | Radio B Chip Enable |
| | CSN | `GPIO 10`| Radio B Chip Select |
| **CC1101 (Optional)** | CSN | `GPIO 0` | Sub-GHz Chip Select |
| | GDO0 | `GPIO 2` | Sub-GHz GDO0 |
| | GDO2 | `GPIO 8` | Sub-GHz GDO2 |
| **I2C OLED (Optional)**| SDA | `GPIO 8` | I2C Data |
| | SCL | `GPIO 9` | I2C Clock |

---

## Profile 2: ESP32 DevKit V1 / WROOM-32 (Standard Dual-Core)

```
                    ESP32 DevKit V1 Pinout Map
                         +-----------------+
                         | [Micro/Type-C]  |
                     EN  | EN           23 | GPIO 23 (SPI MOSI)
                     36  | VP           22 | GPIO 22 (I2C SCL)
                     39  | VN            1 | GPIO 1  (UART TX0)
                     34  | 34            3 | GPIO 3  (UART RX0)
                     35  | 35           21 | GPIO 21 (I2C SDA)
                     32  | 32           19 | GPIO 19 (SPI MISO)
        CC1101 GDO2  33  | 33           18 | GPIO 18 (SPI SCK)
        CC1101 GDO0  25  | 25            5 | GPIO 5  (Radio A CSN)
        CC1101 CSN   26  | 26           17 | GPIO 17 (TX2)
        Radio D CSN  27  | 27           16 | GPIO 16 (RX2)
        Radio D CE   14  | 14            4 | GPIO 4  (Radio A CE)
        Radio C CE   13  | 13            0 | GPIO 0  (Boot Button)
        Radio C CSN  12  | 12            2 | GPIO 2  (Radio B CE / Blue LED)
                     GND | GND          15 | GPIO 15 (Radio B CSN)
                     VIN | VIN         3V3 | 3.3V Power Out
                         +-----------------+
```

### Wiring Table:
| Component | Signal | ESP32 GPIO | Notes |
|---|---|---|---|
| **Shared SPI Bus** | SCK | `GPIO 18` | Hardware VSPI SCK |
| | MISO | `GPIO 19` | Hardware VSPI MISO |
| | MOSI | `GPIO 23` | Hardware VSPI MOSI |
| **nRF24 #1 (A)** | CE | `GPIO 4` | Radio A Chip Enable |
| | CSN | `GPIO 5` | Radio A Chip Select |
| **nRF24 #2 (B)** | CE | `GPIO 2` | Radio B Chip Enable |
| | CSN | `GPIO 15`| Radio B Chip Select |
| **nRF24 #3 (C)** | CE | `GPIO 13`| Radio C Chip Enable |
| | CSN | `GPIO 12`| Radio C Chip Select |
| **nRF24 #4 (D)** | CE | `GPIO 14`| Radio D Chip Enable |
| | CSN | `GPIO 27`| Radio D Chip Select |
| **CC1101 Sub-GHz** | CSN | `GPIO 26`| CC1101 Chip Select |
| | GDO0 | `GPIO 25`| CC1101 Digital IO 0 |
| | GDO2 | `GPIO 33`| CC1101 Digital IO 2 |
| **I2C SSD1306 OLED** | SDA | `GPIO 21`| Hardware I2C SDA |
| | SCL | `GPIO 22`| Hardware I2C SCL |

---

## Profile 3: ESP32-S3 DevKit (Dual-Core LX7)

### Wiring Table:
| Component | Signal | ESP32-S3 GPIO |
|---|---|---|
| **Shared SPI Bus** | SCK | `GPIO 12` |
| | MISO | `GPIO 13` |
| | MOSI | `GPIO 11` |
| **nRF24 #1 (A)** | CE | `GPIO 10` |
| | CSN | `GPIO 9` |
| **nRF24 #2 (B)** | CE | `GPIO 4` |
| | CSN | `GPIO 5` |
| **nRF24 #3 (C)** | CE | `GPIO 6` |
| | CSN | `GPIO 7` |
| **nRF24 #4 (D)** | CE | `GPIO 15` |
| | CSN | `GPIO 16` |
| **CC1101 Sub-GHz** | CSN | `GPIO 17` |
| | GDO0 | `GPIO 18` |
| | GDO2 | `GPIO 8` |
| **I2C Display** | SDA | `GPIO 1` |
| | SCL | `GPIO 2` |

---

## nRF24L01+ Module Pinout Reference

```
         nRF24L01+ 8-Pin Header (Top View, Antenna Facing UP)
                          +-------+
                  GND [1] | o   o | [2] VCC (3.3V ONLY!)
                   CE [3] | o   o | [4] CSN
                  SCK [5] | o   o | [6] MOSI
                 MISO [7] | o   o | [8] IRQ (Not used)
                          +-------+
```
