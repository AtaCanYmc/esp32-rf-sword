# Radio Frequency (RF) Reference Guide

A detailed guide to the radio frequency spectrum allocations, channel plans, and protocol characteristics utilized by ESP32-RF-SWORD.

---

## 1. 2.4 GHz ISM Band Spectrum Overview

The 2.4 GHz Industrial, Scientific, and Medical (ISM) band spans from **2400.0 MHz to 2500.0 MHz**.

The nRF24L01+ transceiver addresses channels in 1 MHz increments:
$$\text{Frequency (MHz)} = 2400 + \text{nRF\_Channel}$$

---

## 2. Wi-Fi 802.11 b/g/n Channel Plan (2.4 GHz)

Standard Wi-Fi channels are 20 MHz (or 40 MHz) wide with 5 MHz center frequency spacing:

| Wi-Fi Channel | Center Frequency | Channel Bandwidth (20 MHz) | nRF24 Channel Range | Primary Non-Overlapping |
|---|---|---|---|---|
| **Channel 1** | 2412 MHz | 2401 – 2423 MHz | Ch 1 – 23 | **YES** |
| **Channel 2** | 2417 MHz | 2406 – 2428 MHz | Ch 6 – 28 | No |
| **Channel 3** | 2422 MHz | 2411 – 2433 MHz | Ch 11 – 33 | No |
| **Channel 4** | 2427 MHz | 2416 – 2438 MHz | Ch 16 – 38 | No |
| **Channel 5** | 2432 MHz | 2421 – 2443 MHz | Ch 21 – 43 | No |
| **Channel 6** | 2437 MHz | 2426 – 2448 MHz | Ch 26 – 48 | **YES** |
| **Channel 7** | 2442 MHz | 2431 – 2453 MHz | Ch 31 – 53 | No |
| **Channel 8** | 2447 MHz | 2436 – 2458 MHz | Ch 36 – 58 | No |
| **Channel 9** | 2452 MHz | 2441 – 2463 MHz | Ch 41 – 63 | No |
| **Channel 10**| 2457 MHz | 2446 – 2468 MHz | Ch 46 – 68 | No |
| **Channel 11**| 2462 MHz | 2451 – 2473 MHz | Ch 51 – 73 | **YES** |
| **Channel 12**| 2467 MHz | 2456 – 2478 MHz | Ch 56 – 78 | No (EU/JP only) |
| **Channel 13**| 2472 MHz | 2461 – 2483 MHz | Ch 61 – 83 | No (EU/JP only) |
| **Channel 14**| 2484 MHz | 2473 – 2495 MHz | Ch 73 – 95 | No (Japan 802.11b) |

---

## 3. Bluetooth Low Energy (BLE) Channel Plan

BLE divides the 2.4 GHz band into **40 channels** (2 MHz spacing each):
- **3 Primary Advertising Channels** (Channels 37, 38, 39) used for device discovery, pairing popups, and beacons.
- **37 Data Channels** (Channels 0 to 36) used for active connections with Frequency Hopping Spread Spectrum (FHSS).

| BLE Channel Index | Frequency | Purpose | nRF24 Channel |
|---|---|---|---|
| **BLE Ch 37** | **2402 MHz** | **Primary Advertisement** | **Ch 2** |
| BLE Ch 0 | 2404 MHz | Data Channel | Ch 4 |
| BLE Ch 1 | 2406 MHz | Data Channel | Ch 6 |
| ... | ... | ... | ... |
| **BLE Ch 38** | **2426 MHz** | **Primary Advertisement** | **Ch 26** |
| BLE Ch 11 | 2428 MHz | Data Channel | Ch 28 |
| ... | ... | ... | ... |
| BLE Ch 36 | 2478 MHz | Data Channel | Ch 78 |
| **BLE Ch 39** | **2480 MHz** | **Primary Advertisement** | **Ch 80** |

---

## 4. ZigBee / Thread / 802.15.4 Channel Plan

IEEE 802.15.4 (ZigBee, Thread, Matter) defines 16 channels in the 2.4 GHz band with 5 MHz channel spacing:

| ZigBee Channel | Center Frequency | nRF24 Channel | Fits Between Wi-Fi Channels |
|---|---|---|---|
| **Channel 11** | 2405 MHz | Ch 5 | Below Wi-Fi Ch 1 |
| **Channel 12** | 2410 MHz | Ch 10 | Wi-Fi Ch 1 |
| **Channel 13** | 2415 MHz | Ch 15 | Wi-Fi Ch 1 |
| **Channel 14** | 2420 MHz | Ch 20 | Wi-Fi Ch 1 / Ch 6 gap |
| **Channel 15** | 2425 MHz | Ch 25 | Between Wi-Fi Ch 1 & Ch 6 |
| **Channel 16** | 2430 MHz | Ch 30 | Wi-Fi Ch 6 |
| **Channel 17** | 2435 MHz | Ch 35 | Wi-Fi Ch 6 |
| **Channel 18** | 2440 MHz | Ch 40 | Wi-Fi Ch 6 |
| **Channel 19** | 2445 MHz | Ch 45 | Wi-Fi Ch 6 |
| **Channel 20** | 2450 MHz | Ch 50 | Between Wi-Fi Ch 6 & Ch 11 |
| **Channel 21** | 2455 MHz | Ch 55 | Wi-Fi Ch 11 |
| **Channel 22** | 2460 MHz | Ch 60 | Wi-Fi Ch 11 |
| **Channel 23** | 2465 MHz | Ch 65 | Wi-Fi Ch 11 |
| **Channel 24** | 2470 MHz | Ch 70 | Wi-Fi Ch 11 |
| **Channel 25** | 2475 MHz | Ch 75 | Above Wi-Fi Ch 11 |
| **Channel 26** | 2480 MHz | Ch 80 | Above Wi-Fi Ch 11 (Guard band) |

---

## 5. Sub-GHz ISM Bands (TI CC1101)

| Frequency | Region | Typical Applications |
|---|---|---|
| **315.00 MHz** | North America / Asia | Garage door openers, legacy car key fobs, home alarms |
| **433.92 MHz** | Global / ITU Region 1 | Remote switches, weather stations, tire pressure (TPMS), gate openers |
| **868.35 MHz** | Europe / ITU Region 1 | LoRaWAN, smart metering, Z-Wave, industrial telemetry |
| **915.00 MHz** | North America / Region 2 | LoRaWAN, amateur radio, drone long-range telemetry, smart city sensors |
