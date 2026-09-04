#!/usr/bin/env python3
"""
flash_firmware.py: Automated flashing utility for ESP32-RF-SWORD
"""

import sys
import os
import subprocess
import argparse

def main():
    parser = argparse.ArgumentParser(description="ESP32-RF-SWORD Automated Firmware Flasher")
    parser.add_argument("-e", "--env", default="esp32-c3-supermini", 
                        choices=["esp32-c3-supermini", "esp32-devkit-v1", "esp32-s3-devkit", "esp32-c6-devkit"],
                        help="Target hardware environment")
    parser.add_argument("-p", "--port", default=None, help="Serial port (e.g. /dev/ttyUSB0 or COM3)")
    parser.add_argument("-b", "--baud", default="460800", help="Flashing baud rate (default: 460800)")
    parser.add_argument("--erase", action="store_true", help="Erase entire flash before flashing")
    args = parser.parse_args()

    print("==================================================")
    print("   ESP32-RF-SWORD Firmware Flashing Utility       ")
    print("==================================================")
    print(f"Target Board Environment : {args.env}")
    if args.port:
        print(f"Serial Port              : {args.port}")
    print(f"Baud Rate                : {args.baud}")
    print("--------------------------------------------------")

    cmd = ["pio", "run", "-e", args.env, "--target", "upload"]
    if args.port:
        cmd.extend(["--upload-port", args.port])

    if args.erase:
        print("[*] Erasing flash chip...")
        erase_cmd = ["pio", "run", "-e", args.env, "--target", "erase"]
        if args.port:
            erase_cmd.extend(["--upload-port", args.port])
        subprocess.run(erase_cmd, check=True)

    print(f"[*] Building and uploading firmware for '{args.env}'...")
    try:
        subprocess.run(cmd, check=True)
        print("\n[+] Flashing completed successfully!")
        print("[*] Connect to Serial monitor at 115200 baud or Wi-Fi AP 'SWORD-SECURITY-PORTAL'.")
    except subprocess.CalledProcessError as e:
        print(f"\n[-] Flashing failed with error code {e.returncode}")
        sys.exit(1)

if __name__ == "__main__":
    main()
