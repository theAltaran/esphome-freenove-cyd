# ESPHome Component: Freenove CYD 3.5" Display

Support for the Freenove CYD 3.5" TFT display (ST77922) with ESP32-S3 in ESPHome.

## Features

- ✅ **Display Support**: 480×320 ST77922 driver in portrait or landscape mode
- ✅ **PSRAM Integration**: Uses PSRAM for sprite buffering (smooth animations)
- ✅ **Backlight Control**: PWM-based brightness control via GPIO45
- ✅ **Text Rendering**: Centered text drawing with multiple font sizes
- ✅ **Home Assistant Integration**: Backlight as controllable light entity
- ✅ **Logging**: Debug output for PSRAM detection and display initialization

## Installation

### Via GitHub (Recommended)

Add to your ESPHome configuration:

```yaml
external_components:
  - source: github://altaran/esphome-freenove-cyd@main
    components: [freenove_cyd_display]
```

### Via Local Path

Clone or copy the repository to your ESPHome `custom_components` directory:

```yaml
external_components:
  - source:
      type: local
      path: custom_components/freenove_cyd
    components: [freenove_cyd_display]
```

## Configuration

### Minimal Configuration

```yaml
# Freenove CYD Display component
freenove_cyd_display:
  id: my_display

# Backlight control
output:
  - platform: ledc
    pin: GPIO45
    frequency: 5000 Hz
    id: backlight_pwm

light:
  - platform: monochromatic
    output: backlight_pwm
    name: "Display Backlight"
```

### With Refresh Button

```yaml
button:
  - platform: template
    name: "Refresh Display"
    on_press:
      - lambda: |-
          id(my_display).update();
```

## Component Methods

### `update()`
Redraw and push sprite to display. Useful when content changes.

```yaml
on_press:
  - lambda: |-
      id(my_display).update();
```

## Pin Configuration

| Function | GPIO | Notes |
|----------|------|-------|
| Backlight | 45 | PWM output (0-255) |
| Touch SDA | 11 | I2C (capacitive touch) |
| Touch SCL | 13 | I2C (capacitive touch) |

Display uses QSPI interface (pins 6, 9, 10, 11, 12, 13, 14 - configured via TFT_eSPI).

## PSRAM Usage

Automatically detected and utilized:
- **8MB PSRAM** on ESP32-S3
- Used for sprite buffer (480×320 × 2 bytes = 307.2 KB)
- Remaining PSRAM available for future graphics

Configuration in ESPHome YAML:
```yaml
esp32:
  framework:
    type: esp-idf
    sdkconfig_options:
      CONFIG_ESP32S3_SPIRAM: "y"
      CONFIG_SPIRAM_MODE_OCT: "y"
      CONFIG_SPIRAM_USE: "4"
      CONFIG_SPIRAM_MALLOC_RESERVE_INTERNAL_SIZE: "32768"
```

## Troubleshooting

### Display doesn't initialize
- Check backlight (GPIO45) is HIGH
- Verify ESP32-S3 board type in ESPHome config
- Check serial logs for initialization errors

### PSRAM not detected
- Enable PSRAM in board configuration
- Verify `CONFIG_ESP32S3_SPIRAM: "y"` in sdkconfig_options
- Check physical PSRAM chip is soldered to board

### Component not found
- Verify `source` path in `external_components`
- Ensure all files in correct directory structure
- Try clean build: `esphome clean thermostat-2 && esphome compile thermostat-2.yaml`

## Requirements

- ESPHome ≥ 2024.1.0
- TFT_eSPI library (installed automatically)
- ESP32-S3 board variant

## License

MIT

## Author

@altaran (altaran.us)

## Contributing

Feel free to submit issues or PRs on GitHub.

## See Also

- [TFT_eSPI Documentation](https://github.com/Bodmer/TFT_eSPI)
- [Freenove CYD Repository](https://github.com/Freenove/Freenove_ESP32_S3_WROOM_Board)
- [ESPHome External Components](https://esphome.io/components/external_components.html)
