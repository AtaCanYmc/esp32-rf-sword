# Interactive Serial CLI Reference

ESP32-RF-SWORD features a full ANSI/VT100 interactive command shell accessible over USB CDC or UART Serial at **115200 baud**.

---

## Accessing the CLI

Connect your serial terminal (e.g. `pio device monitor`, `screen /dev/ttyUSB0 115200`, `minicom`, or `PuTTY`) with line endings configured for **CR/LF**.

Upon connection, you will see the interactive prompt:
```text
sword > 
```

---

## Command Table

| Command | Arguments | Description | Example |
|---|---|---|---|
| `help` | None | Lists all available commands and syntax | `help` |
| `status` | None | Displays live telemetry, uptime, free heap, active channels, hop rate | `status` |
| `start` | None | Starts active RF transmission / sweep | `start` |
| `stop` | None | Immediately halts all RF transmission across all radios | `stop` |
| `mode` | `<mode_name>` | Switches operational / attack mode | `mode coprime` |
| `preset` | `<preset_name>` | Applies pre-configured protocol frequency profile | `preset ble-adv` |
| `channels` | `<min> <max>` | Configures custom channel span (0 - 125) | `channels 2 80` |
| `dwell` | `<min_us> <max_us>` | Configures dwell timing jitter range in microseconds | `dwell 100 150` |
| `power` | `<min\|low\|high\|max>`| Sets nRF24 output power level | `power max` |
| `spectrum` | None | Prints real-time ASCII 2.4GHz waterfall/bar graph | `spectrum` |
| `pinout` | None | Displays active GPIO pin assignments for current board | `pinout` |
| `save` | None | Persists current configuration to Non-Volatile Storage (NVS) | `save` |
| `reset` | None | Restores factory defaults | `reset` |
| `reboot` | None | Restarts the ESP32 microcontroller | `reboot` |

---

## Operational Modes

| Mode Argument | Name | Target Band / Behavior |
|---|---|---|
| `coprime` | Coprime Sweeper | Enhanced RF-KILL V2 multi-radio uniform coprime carrier sweep |
| `linear` | Linear Sweeper | Fast bidirectional ping-pong carrier sweep |
| `random` | Random Sweeper | Pseudo-random Gaussian jitter channel sweep |
| `ble` | Targeted BLE | Focuses energy exclusively on BLE primary advertising channels (37, 38, 39) |
| `wifi` | Targeted Wi-Fi | Sweeps bandwidth of Wi-Fi Channels 1, 6, 11 |
| `zigbee` | Targeted ZigBee | Sweeps 802.15.4 / ZigBee channels 11 to 26 (2405 - 2480 MHz) |
| `drone` | Targeted Drone | Sweeps RC drone FHSS spread-spectrum bands (FlySky, FrSky, ELRS) |
| `subghz` | Targeted Sub-GHz| Continuous Wave (CW) or noise burst on 315/433/868/915 MHz via CC1101 |
| `noise` | Noise Blaster | Floods spectrum with 2Mbps corrupted frame bursts (CRC disabled) |
| `spectrum` | Spectrum Sniffer | Scans 128 channels continuously for RSSI energy detection |
| `beacon` | Wi-Fi Beacon Spam | Native 802.11 beacon frame flooder with custom SSIDs |
| `deauth` | Wi-Fi Deauth Storm| Native 802.11 deauthentication frame flood |
| `popup` | BLE Popup Storm | Native BLE advertisement flood for Apple iOS, Android FastPair, Windows |
| `blitz` | TOTAL BLITZ | Simultaneous multi-radio & all-band synchronized disruption |

---

## Preset Profiles

| Preset Name | Description | Channel Span |
|---|---|---|
| `full` | 2.4 GHz Full ISM Band | 2402 - 2480 MHz (Channels 2 - 80) |
| `ble-adv` | Bluetooth Primary Advertising | 2402, 2426, 2480 MHz (Channels 2, 26, 80) |
| `ble-all` | Bluetooth All 40 Channels | 2402 - 2480 MHz (All Channels) |
| `wifi-1` | Wi-Fi Channel 1 (2412 MHz) | 2401 - 2423 MHz (Channels 1 - 23) |
| `wifi-6` | Wi-Fi Channel 6 (2437 MHz) | 2426 - 2448 MHz (Channels 26 - 48) |
| `wifi-11` | Wi-Fi Channel 11 (2462 MHz) | 2451 - 2473 MHz (Channels 51 - 73) |
| `wifi-all` | Wi-Fi All Channels 1 - 13 | 2401 - 2473 MHz (Channels 1 - 73) |
| `zigbee` | ZigBee / Thread 802.15.4 | 2405 - 2480 MHz (Channels 11 - 26) |
| `flysky` | FlySky AFHDS RC Drones | 2405 - 2480 MHz |
| `frsky` | FrSky ACCST RC Drones | 2404 - 2478 MHz |
| `elrs` | ExpressLRS 2.4 GHz | 2402 - 2480 MHz |
| `sub-315` | Sub-GHz 315.00 MHz Gate ISM | 315.00 MHz |
| `sub-433` | Sub-GHz 433.92 MHz Car/Alarm | 433.92 MHz |
| `sub-868` | Sub-GHz 868.35 MHz EU ISM | 868.35 MHz |
| `sub-915` | Sub-GHz 915.00 MHz US ISM | 915.00 MHz |
| `custom` | Custom Channel / Dwell Range| User-defined |
