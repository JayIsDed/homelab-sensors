# Setup Guide

## Prerequisites

- Docker + Docker Compose on homelab server
- Traefik reverse proxy with Cloudflare DNS-01 challenge
- PlatformIO CLI or VS Code + PlatformIO extension
- Domain with Cloudflare DNS (e.g., `pancake3d.com`)

## 1. DNS Records (Cloudflare)

| Record | Type | Name | Value | Proxy |
|--------|------|------|-------|-------|
| A | `mqtt` | Server IP | DNS only (gray cloud) |
| A | `grafana` | Server IP | Proxied (orange cloud) |
| A | `influxdb` | Server IP | Proxied (orange cloud) |

**Important:** `mqtt` must be DNS-only because Cloudflare cannot proxy TCP:8883.

## 2. Backend Deployment

### Configure Environment

Add to `/opt/homelab/.env`:

```env
# InfluxDB
INFLUXDB_USER=admin
INFLUXDB_PASSWORD=<strong-password>
INFLUXDB_ORG=homelab
INFLUXDB_BUCKET=sensors
INFLUXDB_TOKEN=<generate-a-token>

# Grafana
GRAFANA_USER=admin
GRAFANA_PASSWORD=<strong-password>

# MQTT
MQTT_PASS_TELEGRAF=<strong-password>
```

### Generate MQTT Users

```bash
cd ~/git/homelab-sensors
./scripts/generate-mqtt-users.sh 10
```

This creates `/opt/homelab/sensors/mosquitto/config/passwd` with sensor-01 through sensor-10 plus a telegraf user.

### Deploy Services

```bash
cd /opt/homelab
docker compose up -d mosquitto telegraf influxdb grafana
```

### Verify Backend

```bash
# Test MQTT (install mosquitto-clients)
mosquitto_pub -h mqtt.pancake3d.com -p 8883 \
  --capath /etc/ssl/certs \
  -u sensor-01 -P <password> \
  -t homelab/sensors/sensor-01/data \
  -m '{"sensor_id":"sensor-01","temperature":22.5,"humidity":45.0}'

# Check InfluxDB
curl -s https://influxdb.pancake3d.com/health

# Open Grafana
# https://grafana.pancake3d.com
```

## 3. Firmware

### Build & Flash

```bash
cd ~/git/homelab-sensors/firmware

# Build
pio run

# Upload firmware
pio run -t upload

# Upload CA certificate (LittleFS)
pio run -t uploadfs
```

### Provision Sensor

See [BLE Provisioning](ble-provisioning.md) for detailed steps.

## 4. Firewall

Ensure these ports are open on the homelab server:

| Port | Protocol | Service |
|------|----------|---------|
| 80 | TCP | HTTP (redirect to HTTPS) |
| 443 | TCP | HTTPS (Grafana, InfluxDB, Obsidian) |
| 8883 | TCP | MQTTS (sensor connections) |

## 5. Grafana Dashboard

A pre-built dashboard is auto-provisioned at startup. Navigate to:

`https://grafana.pancake3d.com` → Dashboards → Sensors → Homelab Sensors

Panels:
- Temperature (°C) — per sensor, time series
- Humidity (%) — per sensor, time series
- WiFi Signal (dBm) — connection quality
- Battery Voltage (V) — for future battery operation
- Sensor Status — online/offline indicator
