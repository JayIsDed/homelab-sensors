# Wiring

## XIAO ESP32-C6 → GY-SHT31-D

```
XIAO ESP32-C6          GY-SHT31-D
──────────────          ──────────
3V3  ──────────────────── VIN
GND  ──────────────────── GND
D4/GPIO6 (SDA) ────────── SDA
D5/GPIO7 (SCL) ────────── SCL
```

## Notes

- **Pull-ups:** Not needed — GY-SHT31-D breakout includes 10K pull-ups
- **Wire length:** Keep under 30cm. 5-8cm ideal for enclosed case
- **I2C address:** 0x44 (default, ADDR pin floating/low)
- **Power:** 3.3V from XIAO regulator (max 600mA, SHT31 draws ~1.5mA)

## Additional Pins

| Pin | Function | Notes |
|-----|----------|-------|
| GPIO15 | Status LED | Onboard LED, active HIGH |
| GPIO2 (D0) | Factory Reset | Ground during boot to wipe config |
| USB-C | Power + Serial | Programming and serial config |

## Pin Map (XIAO ESP32-C6)

```
        USB-C
    ┌───────────┐
D0  │ GPIO2   5V│
D1  │ GPIO3  GND│
D2  │ GPIO4 3V3 │
D3  │ GPIO5 GPIO│ D10
SDA │ GPIO6 GPIO│ D9
SCL │ GPIO7 GPIO│ D8
    └───────────┘
```
