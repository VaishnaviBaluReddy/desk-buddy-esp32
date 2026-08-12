#include "header.h"

#include "lvgl.h"
#include "app.h"

#include <stdio.h>
#include <time.h>

// ==================================================
// HEADER OBJECTS
// ==================================================

static lv_obj_t *date_label = nullptr;
static lv_obj_t *time_label = nullptr;

// ==================================================
// DATETIME UPDATE
// ==================================================

static void header_update_datetime(lv_timer_t *timer)
{
    (void)timer;

    time_t now;
    struct tm time_info;

    time(&now);

    localtime_r(&now, &time_info);

    // --------------------------------------------------
    // Larger buffers to avoid format-truncation warning
    // --------------------------------------------------

    char date_text[64];
    char time_text[32];

    // --------------------------------------------------
    // Date
    // --------------------------------------------------

    snprintf(
        date_text,
        sizeof(date_text),
        "DATE    %02d/%02d/%04d",
        time_info.tm_mday,
        time_info.tm_mon + 1,
        time_info.tm_year + 1900
    );

    // --------------------------------------------------
    // Time
    // --------------------------------------------------

    snprintf(
        time_text,
        sizeof(time_text),
        "TIME    %02d:%02d",
        time_info.tm_hour,
        time_info.tm_min
    );

    // --------------------------------------------------
    // Update labels
    // --------------------------------------------------

    if (date_label != nullptr)
    {
        lv_label_set_text(
            date_label,
            date_text
        );
    }

    if (time_label != nullptr)
    {
        lv_label_set_text(
            time_label,
            time_text
        );
    }
}

// ==================================================
// POWER BUTTON
// ==================================================

static void power_button_clicked(lv_event_t *e)
{
    (void)e;

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
        20,
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
        20,
        40
    );

    // ==================================================
    // POWER BUTTON
    // ==================================================

    lv_obj_t *power_button =
        lv_button_create(parent);

    lv_obj_set_size(
        power_button,
        45,
        45
    );

    lv_obj_align(
        power_button,
        LV_ALIGN_TOP_RIGHT,
        -15,
        15
    );

    // --------------------------------------------------
    // Transparent button
    // --------------------------------------------------

    lv_obj_set_style_bg_opa(
        power_button,
        LV_OPA_TRANSP,
        0
    );

    lv_obj_set_style_border_width(
        power_button,
        0,
        0
    );

    lv_obj_set_style_shadow_width(
        power_button,
        0,
        0
    );

    // ==================================================
    // POWER SYMBOL
    // ==================================================

    lv_obj_t *power_label =
        lv_label_create(power_button);

    lv_label_set_text(
        power_label,
        LV_SYMBOL_POWER
    );

    lv_obj_set_style_text_color(
        power_label,
        lv_color_black(),
        0
    );

    lv_obj_center(
        power_label
    );

    // ==================================================
    // CLICK
    // ==================================================

    lv_obj_add_event_cb(
        power_button,
        power_button_clicked,
        LV_EVENT_CLICKED,
        nullptr
    );

    // ==================================================
    // DATETIME TIMER
    // ==================================================

    lv_timer_create(
        header_update_datetime,
        1000,
        nullptr
    );

    // Update immediately instead of waiting 1 second
    header_update_datetime(nullptr);
}