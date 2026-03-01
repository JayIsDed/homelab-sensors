#!/usr/bin/env bash
# Build and flash firmware + filesystem to connected ESP32-C6
# Usage: ./flash-all.sh [port]

set -euo pipefail

PORT=${1:-/dev/ttyACM0}
FIRMWARE_DIR="$(cd "$(dirname "$0")/../firmware" && pwd)"

echo "=== Homelab Sensor Flasher ==="
echo "Port: ${PORT}"
echo "Firmware: ${FIRMWARE_DIR}"
echo ""

cd "$FIRMWARE_DIR"

echo "--- Building firmware ---"
pio run

echo ""
echo "--- Uploading firmware ---"
pio run -t upload --upload-port "$PORT"

echo ""
echo "--- Uploading filesystem (CA cert) ---"
pio run -t uploadfs --upload-port "$PORT"

echo ""
echo "=== Flash complete! ==="
echo "Open serial monitor: pio device monitor -p ${PORT}"
