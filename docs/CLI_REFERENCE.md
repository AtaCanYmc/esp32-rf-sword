<!-- well-document · audit scorecard: G1:5 G2:5 G3:5 G4:5 G5:5 G6:5 · STATUS: PASS -->

[← Back to Root Repository](../README.md)

# Interactive Serial CLI Reference

ESP32-RF-SWORD features an interactive ANSI/VT100 command shell accessible over USB CDC or UART Serial at **115200 baud**.

---

## Accessing the CLI

Connect your serial terminal (such as `pio device monitor`, `screen /dev/ttyUSB0 115200`, `minicom`, or `PuTTY`) with line endings configured for **CR/LF**.

Upon connection, the interactive prompt is available:
```text
sword > 
```

---

## Command Table

| Command | Arguments | Description | Example |
| :--- | :--- | :--- | :--- |
| `help` | None | Lists all available commands and syntax. | `help` |
| `status` | None | Displays live telemetry, uptime, free heap, active channels, hop rate. | `status` |
| `start` | None | Starts active RF transmission or sweep. | `start` |
| `stop` | None | Halts all RF transmission across all connected transceivers. | `stop` |
| `mode` | `<mode_name>` | Switches operational transmission mode. | `mode coprime` |
| `preset` | `<preset_name>` | Applies pre-configured protocol frequency profile. | `preset ble-adv` |
| `channels` | `<min> <max>` | Configures custom channel span (0 to 125). | `channels 2 80` |
| `dwell` | `<min_us> <max_us>` | Configures dwell timing jitter range in microseconds. | `dwell 100 150` |
| `power` | `<min\|low\|high\|max>`| Sets transceiver output power level. | `power max` |
| `spectrum` | None | Prints real-time ASCII 2.4 GHz waterfall bar graph. | `spectrum` |
| `pinout` | None | Displays active GPIO pin assignments for current board. | `pinout` |
| `save` | None | Persists current configuration to Non-Volatile Storage (NVS). | `save` |
| `reset` | None | Restores factory default configuration. | `reset` |
| `reboot` | None | Executes software microcontroller reset. | `reboot` |

---

## Operational Modes

| Mode Argument | Name | Target Band / Behavior |
| :--- | :--- | :--- |
| `coprime` | Coprime Sweeper | Multi-radio uniform coprime carrier sweep. |
| `linear` | Linear Sweeper | Bidirectional ping-pong carrier sweep. |
| `random` | Random Sweeper | Pseudo-random Gaussian jitter channel sweep. |
| `ble` | Targeted BLE | Focuses energy exclusively on BLE primary advertising channels (37, 38, 39). |
| `wifi` | Targeted Wi-Fi | Sweeps bandwidth of Wi-Fi Channels 1, 6, 11. |
| `zigbee` | Targeted ZigBee | Sweeps 802.15.4 / ZigBee channels 11 to 26 (2405 to 2480 MHz). |
| `drone` | Targeted Drone | Sweeps RC drone FHSS spread-spectrum bands (FlySky, FrSky, ELRS). |
| `subghz` | Targeted Sub-GHz| Continuous Wave (CW) or noise burst on 315/433/868/915 MHz via CC1101. |
| `noise` | Noise Blaster | Transmits 2Mbps corrupted frame bursts with CRC disabled. |
| `spectrum` | Spectrum Sniffer | Scans 128 channels continuously for RSSI energy detection. |
| `beacon` | Wi-Fi Beacon Spam | Native 802.11 beacon frame flooder with custom SSIDs. |
| `deauth` | Wi-Fi Deauth Storm| Native 802.11 deauthentication frame flood. |
| `popup` | BLE Popup Storm | Native BLE advertisement flood for Apple iOS, Android FastPair, and Windows. |
| `blitz` | Synchronized Blitz | Synchronized multi-radio and multi-band transmission profile. |

---

## Preset Profiles

| Preset Name | Description | Channel Span |
| :--- | :--- | :--- |
| `full` | 2.4 GHz Full ISM Band | 2402 to 2480 MHz (Channels 2 to 80) |
| `ble-adv` | Bluetooth Primary Advertising | 2402, 2426, 2480 MHz (Channels 2, 26, 80) |
| `ble-all` | Bluetooth All 40 Channels | 2402 to 2480 MHz (All Channels) |
| `wifi-1` | Wi-Fi Channel 1 (2412 MHz) | 2401 to 2423 MHz (Channels 1 to 23) |
| `wifi-6` | Wi-Fi Channel 6 (2437 MHz) | 2426 to 2448 MHz (Channels 26 to 48) |
| `wifi-11` | Wi-Fi Channel 11 (2462 MHz) | 2451 to 2473 MHz (Channels 51 to 73) |
| `wifi-all` | Wi-Fi All Channels 1 - 13 | 2401 to 2473 MHz (Channels 1 to 73) |
| `zigbee` | ZigBee / Thread 802.15.4 | 2405 to 2480 MHz (Channels 11 to 26) |
| `flysky` | FlySky AFHDS RC Drones | 2405 to 2480 MHz |
| `frsky` | FrSky ACCST RC Drones | 2404 to 2478 MHz |
| `elrs` | ExpressLRS 2.4 GHz | 2402 to 2480 MHz |
| `sub-315` | Sub-GHz 315.00 MHz Gate ISM | 315.00 MHz |
| `sub-433` | Sub-GHz 433.92 MHz Alarm ISM | 433.92 MHz |
| `sub-868` | Sub-GHz 868.35 MHz EU ISM | 868.35 MHz |
| `sub-915` | Sub-GHz 915.00 MHz US ISM | 915.00 MHz |
| `custom` | Custom Channel / Dwell Range| User-defined |
