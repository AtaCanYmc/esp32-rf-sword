#!/usr/bin/env python3
"""
merge_bin.py: Generates a single unified flashable binary (0x0000) for ESP32 Web Flasher and esptool
"""

import os
import sys
import shutil
import subprocess
import argparse

def find_esptool():
    # 1. Check PlatformIO penv virtual environment
    pio_penv_esptool = os.path.expanduser("~/.platformio/penv/bin/esptool.py")
    if os.path.isfile(pio_penv_esptool) and os.access(pio_penv_esptool, os.X_OK):
        return [pio_penv_esptool]

    # 2. Check in PATH
    if shutil.which("esptool.py"):
        return ["esptool.py"]
    if shutil.which("esptool"):
        return ["esptool"]
    
    # 3. Check in PlatformIO tool-esptoolpy
    pio_esptool = os.path.expanduser("~/.platformio/packages/tool-esptoolpy/esptool.py")
    if os.path.isfile(pio_esptool):
        return [sys.executable, pio_esptool]

    # 4. Fallback to python module
    return [sys.executable, "-m", "esptool"]

def merge(env_name, output_dir="dist"):
    build_dir = os.path.join(".pio", "build", env_name)
    if not os.path.isdir(build_dir):
        print(f"[-] Error: Build directory '{build_dir}' not found. Run 'pio run -e {env_name}' first.")
        sys.exit(1)

    os.makedirs(output_dir, exist_ok=True)
    out_bin = os.path.join(output_dir, f"esp32-rf-sword-{env_name}-merged.bin")

    chip = "esp32"
    bootloader_offset = "0x1000"
    if "c3" in env_name:
        chip = "esp32c3"
        bootloader_offset = "0x0000"
    elif "s3" in env_name:
        chip = "esp32s3"
        bootloader_offset = "0x0000"
    elif "c6" in env_name:
        chip = "esp32c6"
        bootloader_offset = "0x0000"

    bootloader = os.path.join(build_dir, "bootloader.bin")
    partitions = os.path.join(build_dir, "partitions.bin")
    firmware = os.path.join(build_dir, "firmware.bin")

    # Find boot_app0 if present
    boot_app0 = None
    possible_boot_app = os.path.expanduser("~/.platformio/packages/framework-arduinoespressif32/tools/partitions/boot_app0.bin")
    if os.path.isfile(possible_boot_app):
        boot_app0 = possible_boot_app

    esptool_cmd = find_esptool()
    cmd = esptool_cmd + [
        "--chip", chip,
        "merge_bin",
        "-o", out_bin,
        "--flash_mode", "dio",
        "--flash_size", "4MB",
        bootloader_offset, bootloader,
        "0x8000", partitions,
    ]

    if boot_app0:
        cmd.extend(["0xe000", boot_app0])

    cmd.extend(["0x10000", firmware])

    print(f"[*] Merging binaries for '{env_name}' ({chip})...")
    try:
        subprocess.run(cmd, check=True)
        print(f"[+] Merged unified binary generated: {out_bin} ({os.path.getsize(out_bin)} bytes)")
        return out_bin
    except subprocess.CalledProcessError as e:
        print(f"[-] Error during merge: {e}")
        sys.exit(1)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Merge ESP32 binaries into a single flashable file")
    parser.add_argument("-e", "--env", default="esp32-c3-supermini", help="PlatformIO environment name")
    parser.add_argument("-o", "--out", default="dist", help="Output directory")
    args = parser.parse_args()
    merge(args.env, args.out)
