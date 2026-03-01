# homelab-sensors

Distributed IoT temperature and humidity monitoring system.

## Hardware

- 10x [Seeed XIAO ESP32-C6](https://www.seeedstudio.com/Seeed-Studio-XIAO-ESP32C6-p-5884.html)
- 10x [GY-SHT31-D](https://sensirion.com/products/catalog/SHT31) I2C sensor breakout
- 3D-printed snap-fit enclosure

## Stack

```
[ESP32-C6 sensors] --MQTTS--> [Traefik:8883] --TCP--> [Mosquitto]
                                                            |
                                                      [Telegraf] --> [InfluxDB v2]
                                                                          |
                                                                    [Grafana]
```

## Features

- BLE WiFi provisioning (works on any network globally)
- TLS-encrypted MQTT with per-sensor authentication
- Factory reset via hardware pin
- LED status indicators (provisioning/connected/error)
- Pre-built Grafana dashboards
- Parametric 3D-printable enclosure (OpenSCAD)

## Quick Start

See [docs/setup-guide.md](docs/setup-guide.md) for full deployment instructions.

### Backend

```bash
# Add sensor services to homelab docker-compose
# Configure .env with credentials
docker compose up -d mosquitto telegraf influxdb grafana
```

### Firmware

```bash
cd firmware
# Edit include/config.h for defaults
pio run -t upload        # Flash firmware
pio run -t uploadfs      # Upload CA cert (LittleFS)
```

### Provision a Sensor

1. Power on — LED blinks fast (provisioning mode)
2. Open [ESP BLE Prov](https://play.google.com/store/apps/details?id=com.espressif.provble) app
3. Scan → connect → enter PoP PIN → configure WiFi
4. Connect via serial to set MQTT credentials and sensor ID
5. Reboot — LED pulses slowly (connected, reporting)

## Documentation

- [Setup Guide](docs/setup-guide.md)
- [BLE Provisioning](docs/ble-provisioning.md)
- [Wiring Diagram](docs/wiring.md)
- [Architecture](docs/architecture.md)

## License

MIT
