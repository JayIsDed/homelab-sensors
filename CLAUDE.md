# CLAUDE.md — homelab-sensors

## Project Overview

Distributed IoT temperature/humidity monitoring: 10x XIAO ESP32-C6 + SHT31-D sensors → MQTTS → Mosquitto → Telegraf → InfluxDB → Grafana.

## Repository Structure

- `firmware/` — PlatformIO project (XIAO ESP32-C6, Arduino framework)
- `backend/` — Docker config files (deployed to `/opt/homelab/sensors/`)
- `case/` — OpenSCAD parametric enclosure
- `docs/` — Setup guides and architecture docs
- `scripts/` — Utility scripts (MQTT user generation, bulk flash)

## Key Architecture

- Sensors publish JSON to `homelab/sensors/{sensor_id}/data` over MQTTS (port 8883)
- Traefik terminates TLS for MQTT via TCP router (HostSNI)
- Mosquitto ACL restricts per-sensor write access
- Telegraf subscribes to `homelab/sensors/#` and writes to InfluxDB v2
- Grafana reads from InfluxDB via Flux queries

## Hardware

- **MCU:** Seeed XIAO ESP32-C6 (RISC-V, WiFi 6, BLE 5.0)
- **Sensor:** GY-SHT31-D (I2C: SDA=GPIO6, SCL=GPIO7)
- **LED:** GPIO15 (status indicator)
- **Factory Reset:** GPIO2 (ground during boot)

## Conventions

- Firmware: C++ with Arduino framework, PlatformIO build system
- Backend: Docker Compose services integrated into `/opt/homelab/docker-compose.yml`
- All secrets via `.env` files, never committed
