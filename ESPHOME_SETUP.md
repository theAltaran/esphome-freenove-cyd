# Freenove CYD 3.5" ESPHome Setup
## External Component Integration with Home Assistant

### What's Changed

✅ **Moved from deprecated `custom_component` to `external_components`**
- Cleaner separation of concerns
- Can be versioned and shared independently
- Standard ESPHome external component structure

---

## Setup Options

### Option A: Local Component (Quick testing)

Use this if you're running ESPHome on ubuntu1 locally.

1. **Create component directory** in your ESPHome config:
   ```bash
   mkdir -p config/esphome/custom_components/freenove_cyd/components/freenove_cyd_display
   ```

2. **Copy these files** into that directory:
   - `__init__.py`
   - `freenove_cyd_display.h`
   - `freenove_cyd_display.cpp`

3. **Update your YAML** to reference local path:
   ```yaml
   external_components:
     - source:
         type: local
         path: custom_components/freenove_cyd
       components: [freenove_cyd_display]
   ```

4. **Compile and deploy**

---

### Option B: GitHub Component (Production)

Use this for a reusable, versioned component.

1. **Create a GitHub repo**: `esphome-freenove-cyd`
   ```
   esphome-freenove-cyd/
   ├── components/
   │   └── freenove_cyd_display/
   │       ├── __init__.py
   │       ├── freenove_cyd_display.h
   │       ├── freenove_cyd_display.cpp
   │       └── manifest.json
   ├── manifest.json
   └── README.md
   ```

2. **Reference in YAML**:
   ```yaml
   external_components:
     - source: github://altaran/esphome-freenove-cyd@main
       components: [freenove_cyd_display]
   ```

3. **Compile and deploy via ESPHome dashboard**

---

## YAML Configuration

The `thermostat_display.yaml` now uses:

```yaml
# External component reference
external_components:
  - source: github://altaran/esphome-freenove-cyd@main
    components: [freenove_cyd_display]

# Initialize the display
freenove_cyd_display:
  id: my_display

# Button to trigger display refresh
button:
  - platform: template
    name: "Refresh Display"
    on_press:
      - lambda: |-
          id(my_display).update();
```

---

## Features

✅ **Display Initialization**
- QSPI TFT_eSPI driver
- Portrait mode (320×480 after rotation)
- PSRAM sprite buffer for smooth rendering
- Displays "test" centered on boot

✅ **Backlight Control**
- PWM on GPIO45
- Light entity in Home Assistant
- Brightness transitions & effects

✅ **Logging**
- Debug output for PSRAM detection
- Component setup confirmation
- Text rendering positions logged

---

## Component Structure

```cpp
FreenoveCYDDisplay (ESPHome Component)
├── setup()           // Initialize display, draw "test"
├── loop()            // Called continuously by ESPHome
├── update()          // Redraw sprite and push to display
└── draw_text()       // Helper to draw centered text
```

### Key Methods

**`setup()`**
- Initializes TFT_eSPI driver
- Creates PSRAM sprite buffer
- Sets rotation to portrait
- Draws initial "test" text

**`update()`**
- Redraws sprite to display
- Call this when content changes
- Can be triggered from YAML via lambda

**`draw_text(text, x, y, color)`**
- Draws text centered at (x, y)
- Auto-centers based on text bounds
- Supports any 16-bit RGB565 color

---

## Deployment Steps

### 1. Local Setup (for ubuntu1)

```bash
# Create component directory
cd /path/to/esphome/config
mkdir -p custom_components/freenove_cyd/components/freenove_cyd_display

# Copy files
cp __init__.py custom_components/freenove_cyd/components/freenove_cyd_display/
cp freenove_cyd_display.* custom_components/freenove_cyd/components/freenove_cyd_display/
cp manifest.json custom_components/freenove_cyd/
```

Then in `thermostat_display.yaml`:
```yaml
external_components:
  - source:
      type: local
      path: custom_components/freenove_cyd
    components: [freenove_cyd_display]
```

### 2. ESPHome Dashboard

- Add new device or edit existing
- Upload `thermostat_display.yaml`
- Compile → Install → Monitor

### 3. Home Assistant

- New device should appear in Settings → Devices & Services
- Light entity for backlight available
- Refresh button to test display update

---

## Troubleshooting

### "Cannot find freenove_cyd_display component"
- ✅ Check `external_components` source path
- ✅ Verify all `.py` and `.cpp` files in correct location
- ✅ Ensure `manifest.json` present
- ✅ Rebuild/clean build

### Display doesn't show anything
- ✅ Check backlight is on (GPIO45 HIGH)
- ✅ Monitor ESPHome logs for setup errors
- ✅ Verify board variant: `ESP32-S3-DevKit-C-1`
- ✅ PSRAM enabled in board config

### PSRAM not detected
- ✅ Verify ESPHome is using correct board type
- ✅ Check `CONFIG_ESP32S3_SPIRAM: "y"` in YAML
- ✅ Review boot logs in Serial Monitor

---

## Next Steps

Once working:

1. **Add touchscreen support**
   - Read capacitive touch on GPIO11/13
   - Create touch-triggered actions

2. **Fetch Home Assistant data**
   - Pull thermostat temp from HA
   - Display dynamic content

3. **Create UI elements**
   - Temperature display
   - Set point controls
   - Schedule interface

4. **Custom components**
   - Weather widget
   - Clock display
   - Alert notifications

---

## File Reference

| File | Purpose |
|------|---------|
| `thermostat_display.yaml` | Main ESPHome config |
| `__init__.py` | ESPHome component schema/config |
| `freenove_cyd_display.h` | C++ header |
| `freenove_cyd_display.cpp` | C++ implementation |
| `manifest.json` | Component metadata |

---

## References

- [ESPHome External Components](https://esphome.io/components/external_components.html)
- [TFT_eSPI Library](https://github.com/Bodmer/TFT_eSPI)
- [ESP32-S3 PSRAM](https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/api-reference/system/mem_alloc.html)
