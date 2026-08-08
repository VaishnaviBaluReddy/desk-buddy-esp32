# Desk Buddy ESP32-S3

Personal ESP32-S3 project for a cute productivity/desk companion with an LVGL-based UI.

The current hardware target is the **Waveshare ESP32-S3 4.3" RGB LCD Touch Display (800×480)**.

---

## Current Status

✅ ESP-IDF 5.5.5 configured  
✅ ESP32-S3 target configured  
✅ Waveshare RGB LCD initialized  
✅ GT911 touch dependency configured  
✅ ESP LVGL Adapter configured  
✅ LVGL running on physical hardware  
✅ Custom image/sprite assets compiled into firmware  
✅ Sprite successfully displayed on the physical LCD  
⬜ Animated pet/UI polish  
⬜ Touch interaction  
⬜ Task/habit system  
⬜ Wi-Fi  
⬜ Weather  
⬜ Calendar  
⬜ Firebase/backend integration  
⬜ Final enclosure/hardware

**Important:** The display pipeline is currently working on the actual ESP32-S3 hardware.

---

# Hardware

Target board:

- Waveshare ESP32-S3 4.3" Capacitive Touch Display
- Resolution: **800 × 480**
- RGB LCD
- GT911 capacitive touch controller
- ESP32-S3

Current display configuration:

```text
Resolution:        800 × 480
RGB data width:    16-bit
Pixel clock:       16 MHz
Touch controller:  GT911
````

---

# Software

## ESP-IDF

Current version:

```text
ESP-IDF v5.5.5
```

Example location on this machine:

```text
E:/esp/v5.5.5/esp-idf
```

ESP-IDF environment should be activated before working on the project.

Example:

```powershell
cd E:\esp\v5.5.5\esp-idf
.\export.ps1
```

Then return to the project:

```powershell
cd E:\Projects\desk-buddy-esp32
```

---

# Project Structure

```text
desk-buddy-esp32/
│
├── main/
│   ├── main.cpp
│   │
│   ├── app/
│   │   ├── app.cpp
│   │   ├── app.h
│   │   └── Assets/
│   │       ├── sprite_0.c
│   │       ├── sprite_0.h
│   │       ├── sprite_0.png
│   │       ├── sprite_1.c
│   │       ├── sprite_1.h
│   │       ├── sprite_1.png
│   │       ├── sprite_2.c
│   │       ├── sprite_2.h
│   │       ├── sprite_2.png
│   │       ├── sprite_3.c
│   │       └── sprite_3.h
│   │
│   └── CMakeLists.txt
│
├── components/
│   └── waveshare_rgb_lcd/
│       ├── CMakeLists.txt
│       └── waveshare_rgb_lcd/
│           ├── waveshare_rgb_lcd_port.c
│           └── waveshare_rgb_lcd_port.h
│
├── managed_components/
│   ├── espressif__esp_lcd_touch/
│   ├── espressif__esp_lcd_touch_gt911/
│   └── espressif__esp_lvgl_adapter/
│
├── sdkconfig
├── sdkconfig.defaults
├── CMakeLists.txt
└── README.md
```

---

# Important Components

The project currently depends on:

### LVGL

LVGL provides the UI framework.

The project uses:

```cpp
#include "lvgl.h"
```

Do **not** blindly replace LVGL include paths throughout the LVGL source tree.

A previous accidental replacement of:

```cpp
#include "lvgl/lvgl.h"
```

with:

```cpp
#include "lvgl.h"
```

modified ~159 files and caused unnecessary debugging.

**Do not repeat this.**

---

### Waveshare RGB LCD component

Location:

```text
components/waveshare_rgb_lcd/
```

This contains the board-specific LCD initialization code.

Main functions:

```cpp
waveshare_esp32_s3_rgb_lcd_init(...)
waveshare_rgb_lcd_backlight_on()
```

---

### ESP LCD Touch

The Waveshare component requires:

```text
espressif__esp_lcd_touch
espressif__esp_lcd_touch_gt911
```

These provide:

```cpp
#include "esp_lcd_touch.h"
#include "esp_lcd_touch_gt911.h"
```

---

### ESP LVGL Adapter

The Waveshare initialization also uses:

```cpp
#include "esp_lv_adapter.h"
```

This is provided by:

```text
espressif__esp_lvgl_adapter
```

The Waveshare component therefore needs all three dependencies declared in its `CMakeLists.txt`.

---

# Waveshare Component CMakeLists

Current structure:

```cmake
idf_component_register(
    SRCS
        "waveshare_rgb_lcd/waveshare_rgb_lcd_port.c"

    INCLUDE_DIRS
        "waveshare_rgb_lcd"

    REQUIRES
        esp_lcd
        esp_driver_i2c
        espressif__esp_lcd_touch
        espressif__esp_lcd_touch_gt911
        espressif__esp_lvgl_adapter
)
```

If one of the following errors appears:

```text
fatal error: esp_lcd_touch.h: No such file or directory
```

```text
fatal error: esp_lcd_touch_gt911.h: No such file or directory
```

```text
fatal error: esp_lv_adapter.h: No such file or directory
```

check the `REQUIRES` list first.

---

# Main Component

Current `main/CMakeLists.txt`:

```cmake
idf_component_register(
    SRCS
        "main.cpp"
        "app/app.cpp"
        "app/Assets/sprite_0.c"
        "app/Assets/sprite_1.c"
        "app/Assets/sprite_2.c"
        "app/Assets/sprite_3.c"

    INCLUDE_DIRS
        "."
        "app"
        "app/Assets"

    REQUIRES
        lvgl
        waveshare_rgb_lcd
)
```

## Important

CMake must compile the `.c` sprite files.

If an image produces:

```text
undefined reference to `sprite_3'
```

even though `sprite_3.c` contains:

```c
const lv_image_dsc_t sprite_3 = {
```

then check whether `sprite_3.c` is actually included in the component's `SRCS`.

Check with:

```powershell
Select-String -Path .\build\build.ninja -Pattern "sprite_3.c"
```

If nothing is returned, CMake is not compiling that file.

---

# Image Assets

Sprites are converted into LVGL C image assets.

Example:

```c
const lv_image_dsc_t sprite_3 = {
    .header.cf = LV_COLOR_FORMAT_ARGB8888,
    .header.magic = LV_IMAGE_HEADER_MAGIC,
    .header.w = 64,
    .header.h = 64,
    .data_size = 4096 * 4,
    .data = sprite_3_map,
};
```

The actual pixel data is stored in:

```c
const uint8_t sprite_3_map[] = {
    ...
};
```

Therefore, once compiled and flashed, the image is stored in the ESP32 firmware.

It does **not** need to be loaded from the computer at runtime.

---

# Current App

`app.cpp` currently displays four sprite frames:

```cpp
static const lv_image_dsc_t* frames[] =
{
    &sprite_0,
    &sprite_1,
    &sprite_2,
    &sprite_3
};
```

The animation timer changes the frame:

```cpp
static void animate_cat(lv_timer_t*)
{
    current_frame++;

    if(current_frame >= 4)
        current_frame = 0;

    lv_image_set_src(cat, frames[current_frame]);
}
```

The image is created with:

```cpp
cat = lv_image_create(lv_screen_active());

lv_image_set_src(cat, frames[0]);

lv_obj_center(cat);

lv_image_set_scale(cat, 512);
```

Current animation interval:

```text
500 ms
```

---

# Display Initialization

`main.cpp` initializes the hardware before starting the application.

High-level flow:

```text
ESP32 boot
    ↓
Initialize Waveshare RGB LCD
    ↓
Turn on backlight
    ↓
Initialize ESP LVGL Adapter
    ↓
Register RGB display
    ↓
Register GT911 touch
    ↓
Start LVGL adapter
    ↓
Lock LVGL
    ↓
app_start()
    ↓
Create UI
```

The important application boundary is:

```cpp
app_start();
```

Future UI/application logic should generally live in `app/`, rather than being mixed into the hardware initialization code.

---

# Building

From the project directory:

```powershell
idf.py build
```

If the build succeeds:

```text
Project build complete.
```

---

# Flashing

Connect the ESP32-S3 to the computer.

Then:

```powershell
idf.py flash
```

Or build + flash:

```powershell
idf.py build flash
```

---

# Monitoring Serial Output

```powershell
idf.py monitor
```

Or:

```powershell
idf.py flash monitor
```

To exit the monitor:

```text
Ctrl+C
```

---

# Disconnecting the Board

The ESP32 does **not** need to be safely "ejected" like a USB drive.

If the monitor is running:

```text
Ctrl+C
```

Then disconnect the USB cable.

The firmware is already stored in the ESP32's flash memory.

The board can then be powered from:

* USB power adapter
* Power bank
* Other suitable USB power source

The flashed firmware should boot without the computer.

---

# Clean Builds

Normally:

```powershell
idf.py build
```

is enough.

If CMake/component state becomes stale:

```powershell
idf.py fullclean
idf.py build
```

If necessary, the `build/` directory can also be removed manually and regenerated.

**Do not delete source files or managed components just because a clean build fails.**

---

# Troubleshooting

## `undefined reference to sprite_X`

Example:

```text
undefined reference to `sprite_3'
```

Check:

1. `sprite_3.c` exists.
2. `sprite_3.c` contains the definition.
3. `sprite_3.h` contains the `extern`.
4. `app.cpp` includes the correct header.
5. `sprite_3.c` is listed in `SRCS` in `main/CMakeLists.txt`.

Check:

```powershell
Get-Content .\main\app\Assets\sprite_3.c
```

and:

```powershell
Select-String -Path .\build\build.ninja -Pattern "sprite_3.c"
```

---

## `esp_lcd_touch.h: No such file or directory`

Add:

```text
espressif__esp_lcd_touch
```

to the Waveshare component's `REQUIRES`.

---

## `esp_lcd_touch_gt911.h: No such file or directory`

Add:

```text
espressif__esp_lcd_touch_gt911
```

to `REQUIRES`.

---

## `esp_lv_adapter.h: No such file or directory`

Add:

```text
espressif__esp_lvgl_adapter
```

to `REQUIRES`.

---

## CMake says an include directory doesn't exist

Example:

```text
Include directory '.../main/Assests' is not a directory.
```

Check spelling carefully.

The project uses:

```text
Assets
```

not:

```text
Assests
```

Also check whether the path should be:

```text
app/Assets
```

rather than:

```text
Assets
```

depending on the actual folder location.

---

# Git Safety

After reaching a working hardware state, commit immediately.

Example:

```powershell
git status
git add .
git commit -m "Get ESP32-S3 Waveshare display and LVGL sprite working"
git push
```

This working state is important because the display initialization required several dependencies and configuration changes.

---

# Current Known-Good State

As of this checkpoint:

```text
ESP-IDF:             5.5.5
Target:               ESP32-S3
Display:              800x480 RGB
Touch:                GT911
UI framework:         LVGL
LVGL adapter:         ESP LVGL Adapter
Custom sprites:       Working
Physical display:     Working
Sprite displayed:     YES
```

The ESP32 successfully displays the custom sprite on the physical Waveshare LCD.

**Treat this as a known-good checkpoint.**

---

# Next Steps

The intended Desk Buddy architecture is:

```text
Desk Buddy
│
├── app/
│   ├── state machine
│   ├── scheduler
│   └── screen manager
│
├── services/
│   ├── firebase
│   ├── weather
│   ├── calendar
│   └── quotes
│
├── network/
│   ├── wifi
│   └── http
│
├── display/
│   ├── lvgl
│   ├── widgets
│   └── themes
│
└── hardware/
    ├── buttons
    ├── touch
    └── sensors
```

Immediate development priority:

1. Keep the current display initialization untouched.
2. Build the UI inside `app/`.
3. Replace the temporary cat test with the actual Desk Buddy home screen.
4. Implement sprite animation cleanly.
5. Add touch interaction.
6. Add Wi-Fi.
7. Add backend services.
8. Add productivity features.
9. Add sensors/audio/vibration later.

---

# IMPORTANT: Don't Break the Working Display

The hardware initialization is now working.

Before making major changes:

```powershell
git status
git add .
git commit -m "Known good ESP32 display checkpoint"
```

If a future change breaks the display, return to this commit before debugging unrelated changes.

The goal from this point forward is to **build the Desk Buddy application on top of the working display**, rather than repeatedly modifying the hardware/display setup.

```
```
