idf_component_register(
    SRCS
        "main.cpp"

        "app/app.cpp"
        "app/start_screen.cpp"
        "app/task_manager.cpp"
        "app/task_screen.cpp"
        "app/cat/cat_animation.cpp"
        "app/header/header.cpp"
        "network/wifi.cpp"

        "Assets/sleep/sprite_0.c"
        "Assets/sleep/sprite_1.c"
        "Assets/sleep/sprite_2.c"
        "Assets/sleep/sprite_3.c"
        "Assets/sit/sitting_0.c"
        "Assets/sit/sitting_1.c"
        "Assets/sit/sitting_2.c"
        "Assets/sit/sitting_3.c"
        "Assets/sit/sitting_4.c"
        "Assets/sit/sitting_5.c"
        "Assets/stretch/stretching00.c"
        "Assets/stretch/stretching01.c"
        "Assets/stretch/stretching02.c"
        "Assets/stretch/stretching03.c"
        "Assets/stretch/stretching04.c"
        "Assets/stretch/stretching05.c"
        "Assets/stretch/stretching06.c"
        "Assets/stretch/stretching07.c"
        "Assets/stretch/stretching08.c"
        "Assets/stretch/stretching09.c"

    INCLUDE_DIRS
        "."
        "app"
        "Assets"

    REQUIRES
        lvgl
        waveshare_rgb_lcd
        esp_wifi
        esp_event
        esp_netif
        nvs_flash
)