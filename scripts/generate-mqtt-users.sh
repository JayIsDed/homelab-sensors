#!/usr/bin/env bash
# Generate Mosquitto password file for N sensors + telegraf
# Usage: ./generate-mqtt-users.sh [count] [output_dir]
# Passwords are printed to stdout for you to save securely.

set -euo pipefail

COUNT=${1:-10}
OUTPUT_DIR=${2:-/opt/homelab/sensors/mosquitto/config}
PASSWD_FILE="${OUTPUT_DIR}/passwd"

if ! command -v mosquitto_passwd &>/dev/null; then
    echo "Error: mosquitto_passwd not found. Install mosquitto-clients."
    exit 1
fi

# Start fresh
> "$PASSWD_FILE"

echo "=== MQTT Credentials (save these!) ==="
echo ""

# Generate sensor users
for i in $(seq -w 1 "$COUNT"); do
    USER="sensor-${i}"
    PASS=$(openssl rand -base64 16 | tr -d '=/+' | head -c 20)
    mosquitto_passwd -b "$PASSWD_FILE" "$USER" "$PASS"
    echo "${USER}: ${PASS}"
done

# Telegraf user (reads from MQTT_PASS_TELEGRAF env var or generates one)
TELEGRAF_PASS=${MQTT_PASS_TELEGRAF:-$(openssl rand -base64 16 | tr -d '=/+' | head -c 20)}
mosquitto_passwd -b "$PASSWD_FILE" "telegraf" "$TELEGRAF_PASS"
echo ""
echo "telegraf: ${TELEGRAF_PASS}"

echo ""
echo "Password file written to: ${PASSWD_FILE}"
echo "Update MQTT_PASS_TELEGRAF in /opt/homelab/.env with the telegraf password above."
