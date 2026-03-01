# Architecture

## System Overview

```
[ESP32-C6 #1-10] --ESPHome API--> [Home Assistant (RPi)]
                                         |
                                    [InfluxDB v2] (long-term)
                                         |
                                    [Grafana :3000]
                                         |
                        [User] <--HTTPS-- [Traefik :443]
```

## Components

| Component | Role | Location |
|-----------|------|----------|
| XIAO ESP32-C6 | Sensor node (temp/humidity) | Any WiFi network |
| Home Assistant | Central hub, auto-discovery | RPi @ 192.168.8.127 |
| InfluxDB v2 | Long-term time-series storage | Docker @ homelab |
| Grafana | Analytics dashboards | Docker @ homelab |
| Traefik | Reverse proxy, HTTPS | Docker @ homelab |

## Data Flow

1. ESP32-C6 reads SHT31-D sensor via I2C every 60s
2. ESPHome native API pushes state to Home Assistant
3. HA auto-creates entities (temperature, humidity, WiFi signal, uptime)
4. HA recorder stores short-term history (default 10 days)
5. HA InfluxDB integration forwards data for long-term retention
6. Grafana queries InfluxDB for analytics dashboards

## ESPHome Sensor Config

Each sensor is a ~12 line YAML file that inherits from `common/base.yaml`:

```yaml
substitutions:
  device_name: sensor-01
  friendly_name: "Sensor 01"
  location: "living-room"
  update_interval: "60s"
  temp_offset: "0.0"
  humidity_offset: "0.0"

packages:
  base: !include common/base.yaml
```

## WiFi Provisioning

- **First flash:** USB via `esphome run sensor-XX.yaml`
- **WiFi change:** Sensor creates fallback AP (`sensor-XX-setup`) with captive portal
- **OTA updates:** All subsequent flashes are wireless
- **Factory reset:** Hold GPIO2 to GND for 3 seconds

## Security

- ESPHome API encrypted with per-deployment key
- OTA password protected
- Fallback AP password protected
- No credentials in source code (secrets.yaml is gitignored)
- Grafana/InfluxDB behind HTTPS via Traefik
