idf_component_register(
    SRCS
        "waveshare_rgb_lcd/waveshare_rgb_lcd_port.c"

    INCLUDE_DIRS
        "waveshare_rgb_lcd"

    REQUIRES
        esp_lcd
        esp_driver_i2c
        esp_lcd_touch
        esp_lcd_touch_gt911
        esp_lvgl_adapter
)