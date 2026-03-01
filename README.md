# homelab-sensors

Distributed IoT temperature and humidity monitoring system using ESPHome.

## Hardware

- 10x [Seeed XIAO ESP32-C6](https://www.seeedstudio.com/Seeed-Studio-XIAO-ESP32C6-p-5884.html)
- 10x [GY-SHT31-D](https://sensirion.com/products/catalog/SHT31) I2C sensor breakout
- 3D-printed snap-fit enclosure

## Stack

```
[ESP32-C6 sensors] --ESPHome API--> [Home Assistant]
                                         |
                                    [InfluxDB v2] (long-term retention)
                                         |
                                    [Grafana] (dashboards)
```

## Features

- **ESPHome** — YAML-based config, native HA auto-discovery, OTA updates
- **Captive portal** — WiFi fallback AP for easy re-provisioning
- **Factory reset** — Hold GPIO2 for 3 seconds to wipe config
- **Status LED** — GPIO15 shows connection state
- **Web server** — Built-in debug UI on each sensor's IP
- **Calibration** — Per-sensor temperature/humidity offset in YAML
- Parametric 3D-printable enclosure (OpenSCAD)

## Quick Start

### 1. Backend

```bash
# Add sensor services to homelab docker-compose
docker compose up -d influxdb grafana
```

### 2. Flash a Sensor

```bash
cd esphome
cp secrets.yaml.example secrets.yaml  # Fill in WiFi + API keys
esphome run sensor-01.yaml            # Compile + flash via USB
```

### 3. Provision

1. First flash via USB — sensor connects to WiFi automatically
2. Appears in Home Assistant → Integrations → ESPHome
3. Click **Configure** → done
4. For subsequent WiFi changes: sensor creates `sensor-XX-setup` AP

### 4. OTA Updates

After first USB flash, all future updates are wireless:

```bash
esphome run sensor-01.yaml  # Detects sensor on network, uploads OTA
```

## Project Structure

```
esphome/              # ESPHome configs (active)
├── common/base.yaml  # Shared config (WiFi, sensor, LED, etc.)
├── secrets.yaml      # Credentials (gitignored)
├── sensor-01.yaml    # Per-sensor substitutions
└── ...
firmware/legacy/      # Original PlatformIO firmware (archived)
backend/              # Docker configs (Mosquitto, Telegraf, InfluxDB, Grafana)
case/                 # OpenSCAD 3D printable enclosure
docs/                 # Setup guides and architecture
scripts/              # Utility scripts
```

## Documentation

- [Setup Guide](docs/setup-guide.md)
- [Wiring Diagram](docs/wiring.md)
- [Architecture](docs/architecture.md)

## License

MIT
