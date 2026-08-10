#include "header.h"
#include "app.h"

#include "lvgl.h"

#include <stdio.h>
#include <time.h>

// ==================================================
// HEADER OBJECTS
// ==================================================

static lv_obj_t *date_label = nullptr;
static lv_obj_t *time_label = nullptr;
static lv_obj_t *power_button = nullptr;

static lv_timer_t *datetime_timer = nullptr;

// ==================================================
// UPDATE DATE / TIME
// ==================================================

static void header_update_datetime(lv_timer_t *timer)
{
    (void)timer;

    time_t now;

    time(&now);

    struct tm time_info;

    localtime_r(
        &now,
        &time_info
    );

    // --------------------------------------------------
    // Date
    // --------------------------------------------------

    char date_text[64];

    snprintf(
        date_text,
        sizeof(date_text),
        "DATE    %02d/%02d/%04d",
        time_info.tm_mday,
        time_info.tm_mon + 1,
        time_info.tm_year + 1900
    );

    lv_label_set_text(
        date_label,
        date_text
    );

    // --------------------------------------------------
    // Time
    // --------------------------------------------------

    char time_text[32];

    snprintf(
        time_text,
        sizeof(time_text),
        "TIME    %02d:%02d",
        time_info.tm_hour,
        time_info.tm_min
    );

    lv_label_set_text(
        time_label,
        time_text
    );
}

// ==================================================
// POWER BUTTON
// ==================================================

static void power_button_clicked(lv_event_t *e)
{
    (void)e;

    // Navigation is handled by app.cpp.

    app_show_start_screen();
}

// ==================================================
// CREATE HEADER
// ==================================================

void header_create(lv_obj_t *parent)
{
    // ==================================================
    // DATE
    // ==================================================

    date_label =
        lv_label_create(parent);

    lv_label_set_text(
        date_label,
        "DATE    --/--/----"
    );

    lv_obj_set_style_text_color(
        date_label,
        lv_color_black(),
        0
    );

    lv_obj_align(
        date_label,
        LV_ALIGN_TOP_LEFT,
        30,
        15
    );

    // ==================================================
    // TIME
    // ==================================================

    time_label =
        lv_label_create(parent);

    lv_label_set_text(
        time_label,
        "TIME    --:--"
    );

    lv_obj_set_style_text_color(
        time_label,
        lv_color_black(),
        0
    );

    lv_obj_align(
        time_label,
        LV_ALIGN_TOP_LEFT,
        30,
        40
    );

    // ==================================================
    // POWER BUTTON
    // ==================================================

    power_button =
        lv_button_create(parent);

    lv_obj_set_size(
        power_button,
        60,
        60
    );

    lv_obj_align(
        power_button,
        LV_ALIGN_TOP_RIGHT,
        -25,
        15
    );

    lv_obj_set_style_bg_color(
        power_button,
        lv_color_black(),
        0
    );

    lv_obj_set_style_radius(
        power_button,
        4,
        0
    );

    // --------------------------------------------------
    // Power symbol
    // --------------------------------------------------

    lv_obj_t *power_label =
        lv_label_create(power_button);

    lv_label_set_text(
        power_label,
        LV_SYMBOL_POWER
    );

    lv_obj_set_style_text_color(
        power_label,
        lv_color_white(),
        0
    );

    lv_obj_center(power_label);

    // --------------------------------------------------
    // Click
    // --------------------------------------------------

    lv_obj_add_event_cb(
        power_button,
        power_button_clicked,
        LV_EVENT_CLICKED,
        nullptr
    );

    // ==================================================
    // INITIAL DATE/TIME
    // ==================================================

    header_update_datetime(nullptr);

    // ==================================================
    // UPDATE EVERY SECOND
    // ==================================================

    datetime_timer =
        lv_timer_create(
            header_update_datetime,
            1000,
            nullptr
        );
}