# Sensor Provisioning

## First-Time Setup (USB)

1. Wire the sensor ([see wiring guide](wiring.md))
2. Connect XIAO ESP32-C6 via USB-C
3. Set up secrets:
   ```bash
   cd esphome
   cp secrets.yaml.example secrets.yaml
   # Edit secrets.yaml with your WiFi credentials and API keys
   ```
4. Flash:
   ```bash
   esphome run sensor-01.yaml
   ```
5. Sensor connects to WiFi → appears in Home Assistant → Integrations → ESPHome
6. Click **Configure** → **Submit** → done

## Changing WiFi Network

If the configured WiFi is unavailable, the sensor automatically creates a fallback AP:

1. Sensor fails to connect to WiFi (30s timeout)
2. Creates AP: `sensor-XX-setup` (password from `secrets.yaml`)
3. Connect phone/laptop to that AP
4. Captive portal opens → enter new WiFi credentials
5. Sensor reboots and connects to the new network

## OTA Updates

After the initial USB flash, all updates are wireless:

```bash
# ESPHome auto-discovers the sensor on the network
esphome run sensor-01.yaml

# Or specify the IP directly
esphome run sensor-01.yaml --device 192.168.8.XXX
```

## Factory Reset

**Option A — Physical button:**
1. Hold a jumper wire from D0 (GPIO2) to GND
2. Wait 3 seconds
3. Sensor wipes all config and restarts in AP mode

**Option B — From Home Assistant:**
1. Go to the sensor's device page in HA
2. Press the **Factory Reset** button

**Option C — From the web UI:**
1. Navigate to `http://<sensor-ip>` in a browser
2. Use the Factory Reset button

## Per-Sensor Customization

Edit the sensor's YAML file to change:

```yaml
substitutions:
  device_name: sensor-01
  friendly_name: "Living Room"    # Shows in HA
  location: "living-room"         # Metadata
  update_interval: "30s"          # Faster reads
  temp_offset: "-0.5"             # Calibration
  humidity_offset: "2.0"          # Calibration
```

Then OTA flash: `esphome run sensor-01.yaml`
