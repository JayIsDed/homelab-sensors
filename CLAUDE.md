# CLAUDE.md — homelab-sensors

## Project Overview

Distributed IoT temperature/humidity monitoring: 10x XIAO ESP32-C6 + SHT31-D sensors → ESPHome → Home Assistant → InfluxDB → Grafana.

## Repository Structure

- `esphome/` — ESPHome YAML configs (base + per-sensor substitutions)
- `firmware/legacy/` — Archived PlatformIO C++ firmware (replaced by ESPHome)
- `backend/` — Docker config files (deployed to `/opt/homelab/sensors/`)
- `case/` — OpenSCAD parametric enclosure
- `docs/` — Setup guides and architecture docs
- `scripts/` — Utility scripts

## Key Architecture

- Sensors connect to HA via ESPHome native API (auto-discovery, OTA, web UI)
- HA stores short-term history in its recorder database
- InfluxDB v2 stores long-term data (configured via HA InfluxDB integration)
- Grafana reads from InfluxDB for analytics dashboards
- Backend services (Mosquitto/Telegraf) remain available but are optional with ESPHome

## Hardware

- **MCU:** Seeed XIAO ESP32-C6 (RISC-V, WiFi 6, BLE 5.0)
- **Sensor:** GY-SHT31-D (I2C: SDA=GPIO6, SCL=GPIO7)
- **LED:** GPIO15 (status indicator)
- **Factory Reset:** GPIO2 (hold to GND for 3 seconds)

## Conventions

- Firmware: ESPHome YAML with `esp-idf` framework
- Base config in `esphome/common/base.yaml`, per-sensor overrides in `esphome/sensor-XX.yaml`
- Secrets in `esphome/secrets.yaml` (gitignored)
- Backend: Docker Compose services in `/opt/homelab/docker-compose.yml`
