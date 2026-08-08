idf_component_register(
    SRCS
        "main.cpp"
        "app/app.cpp"
        "Assets/sprite_0.c"
        "Assets/sprite_1.c"
        "Assets/sprite_2.c"
        "Assets/sprite_3.c"

    INCLUDE_DIRS
        "."
        "app"
        "Assets"

    REQUIRES
        lvgl
        waveshare_rgb_lcd
)