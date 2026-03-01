# BLE Provisioning

## Overview

Sensors use ESP BLE Provisioning to configure WiFi credentials. This allows deploying sensors to any WiFi network without hardcoding credentials.

## Requirements

- Android: [ESP BLE Prov](https://play.google.com/store/apps/details?id=com.espressif.provble)
- iOS: [ESP BLE Provisioning](https://apps.apple.com/app/esp-ble-provisioning/id1473590141)

## Steps

### 1. Power On (First Boot)

Connect the sensor via USB-C. The LED will blink rapidly (provisioning mode).

Serial output:
```
=== Homelab Sensor ===
[BOOT] Not provisioned, starting BLE...
[BLE] Starting provisioning as 'Sensor-A1B2'
[BLE] PoP PIN: 12345678
```

### 2. Connect via App

1. Open the ESP BLE Prov app
2. Tap **Provision New Device** → **I don't have a QR code**
3. Select **BLE** transport
4. Find your device (e.g., `Sensor-A1B2`)
5. Enter PoP PIN: `12345678`
6. Select your WiFi network and enter password
7. Wait for "Provisioning Successful"

### 3. Configure MQTT (Serial)

After WiFi provisioning, open a serial monitor (115200 baud) and enter:

```
host=mqtt.pancake3d.com
user=sensor-01
pass=your-mqtt-password
id=sensor-01
location=living-room
interval=60
save
```

The sensor reboots and begins reporting.

### 4. Verify

Serial output when running:
```
[WIFI] Connected, IP: 192.168.1.42
[MQTT] Configured: mqtt.pancake3d.com:8883 user=sensor-01
[SENSOR] SHT31 initialized
[MQTT] Connected
[SENSOR] T=22.5°C H=45.2%
[MQTT] Published to homelab/sensors/sensor-01/data
```

## Factory Reset

Ground GPIO2 (D0) during boot to wipe all stored config:
- Hold a jumper wire from D0 to GND
- Power on or press reset
- Release after "Factory reset triggered!" appears
- Sensor re-enters provisioning mode

## Changing WiFi Network

Factory reset the sensor, then re-provision with the new network.
