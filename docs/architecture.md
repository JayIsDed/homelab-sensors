# Architecture

## System Overview

```
[ESP32-C6 #1-10] --MQTTS:8883--> [Traefik:8883] --TCP--> [Mosquitto:1883]
                                                                |
                                                          [Telegraf] --> [InfluxDB:8086]
                                                                               |
                                                                        [Grafana:3000]
                                                                               |
                                                [User] <--HTTPS:443-- [Traefik:443]
```

## Components

| Component | Role | Network |
|-----------|------|---------|
| XIAO ESP32-C6 | Sensor node (temp/humidity) | Any WiFi → MQTTS |
| Traefik | Reverse proxy, TLS termination | Ports 80, 443, 8883 |
| Mosquitto | MQTT broker | Internal (1883) |
| Telegraf | MQTT → InfluxDB bridge | Internal |
| InfluxDB v2 | Time-series database | Internal (8086) |
| Grafana | Dashboard/visualization | Internal (3000) |

## Data Flow

1. ESP32-C6 reads SHT31-D sensor via I2C
2. Publishes JSON to `homelab/sensors/{sensor_id}/data` over MQTTS (port 8883)
3. Traefik terminates TLS at `mqtt.pancake3d.com:8883`, forwards TCP to Mosquitto:1883
4. Mosquitto authenticates sensor, enforces ACL
5. Telegraf subscribes to `homelab/sensors/#`, extracts `sensor_id` from topic
6. Telegraf writes to InfluxDB v2 bucket `sensors`
7. Grafana queries InfluxDB via Flux, displays dashboards

## MQTT Topic Structure

```
homelab/sensors/{sensor_id}/data    # Sensor publishes here
homelab/sensors/#                   # Telegraf subscribes here
```

## Security

- TLS on all external MQTT (Let's Encrypt via Traefik)
- Unique MQTT credentials per sensor
- ACL restricts each sensor to its own topic
- BLE provisioning with Proof of Possession PIN
- No credentials in source code
- Grafana/InfluxDB behind HTTPS only
