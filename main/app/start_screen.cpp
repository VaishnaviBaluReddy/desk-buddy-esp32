#include "start_screen.h"

#include "lvgl.h"

// ==================================================
// ANIMATION SPEEDS
// ==================================================

#define SLEEP_SPEED     500
#define STRETCH_SPEED   350
#define SIT_SPEED       180

// ==================================================
// SLEEPING ANIMATION
// ==================================================

#include "Assets/sleep/sprite_0.h"
#include "Assets/sleep/sprite_1.h"
#include "Assets/sleep/sprite_2.h"
#include "Assets/sleep/sprite_3.h"

// ==================================================
// SITTING ANIMATION
// ==================================================

#include "Assets/sit/sitting_0.h"
#include "Assets/sit/sitting_1.h"
#include "Assets/sit/sitting_2.h"
#include "Assets/sit/sitting_3.h"
#include "Assets/sit/sitting_4.h"
#include "Assets/sit/sitting_5.h"

// ==================================================
// STRETCHING ANIMATION
// ==================================================

#include "Assets/stretch/stretching00.h"
#include "Assets/stretch/stretching01.h"
#include "Assets/stretch/stretching02.h"
#include "Assets/stretch/stretching03.h"
#include "Assets/stretch/stretching04.h"
#include "Assets/stretch/stretching05.h"
#include "Assets/stretch/stretching06.h"
#include "Assets/stretch/stretching07.h"
#include "Assets/stretch/stretching08.h"
#include "Assets/stretch/stretching09.h"

// ==================================================
// CAT STATE
// ==================================================

enum CatState
{
    CAT_SLEEPING,
    CAT_STRETCHING,
    CAT_SITTING
};

static CatState cat_state = CAT_SLEEPING;

// ==================================================
// OBJECTS
// ==================================================

static lv_obj_t *cat = nullptr;

static lv_obj_t *start_button = nullptr;

// ==================================================
// START BUTTON CALLBACK
// ==================================================

static StartButtonCallback start_callback = nullptr;

// ==================================================
// ANIMATION FRAMES
// ==================================================

static const lv_image_dsc_t *sleep_frames[] =
{
    &sprite_0,
    &sprite_1,
    &sprite_2,
    &sprite_3
};

static const lv_image_dsc_t *stretching_frames[] =
{
    &stretching00,
    &stretching01,
    &stretching02,
    &stretching03,
    &stretching04,
    &stretching05,
    &stretching06,
    &stretching07,
    &stretching08,
    &stretching09
};

static const lv_image_dsc_t *sitting_frames[] =
{
    &sitting_0,
    &sitting_1,
    &sitting_2,
    &sitting_3,
    &sitting_4,
    &sitting_5
};

// ==================================================
// ANIMATION VARIABLES
// ==================================================

static uint8_t current_frame = 0;

static lv_timer_t *animation_timer = nullptr;

// ==================================================
// DOUBLE TAP
// ==================================================

static uint8_t tap_count = 0;

static uint32_t first_tap_time = 0;

static const uint32_t DOUBLE_TAP_TIME = 1500;

// ==================================================
// FORWARD DECLARATIONS
// ==================================================

static void animate_cat(
    lv_timer_t *timer
);

static void cat_touched(
    lv_event_t *e
);

static void create_start_button();

static void start_button_clicked(
    lv_event_t *e
);


// ==================================================
// START BUTTON CLICK
// ==================================================

static void start_button_clicked(
    lv_event_t *e
)
{
    (void)e;

    // --------------------------------------------------
    // Do NOT navigate directly here.
    //
    // Tell app.cpp that START was pressed.
    // --------------------------------------------------

    if (start_callback != nullptr)
    {
        start_callback();
    }
}


// ==================================================
// CREATE START BUTTON
// ==================================================

static void create_start_button()
{
    // --------------------------------------------------
    // Don't create it twice.
    // --------------------------------------------------

    if (start_button != nullptr)
    {
        return;
    }

    // ==================================================
    // BUTTON
    // ==================================================

    start_button =
        lv_button_create(
            lv_screen_active()
        );

    lv_obj_set_size(
        start_button,
        180,
        60
    );

    lv_obj_align(
        start_button,
        LV_ALIGN_CENTER,
        0,
        170
    );

    // ==================================================
    // BLACK PIXEL-STYLE BUTTON
    // ==================================================

    lv_obj_set_style_bg_color(
        start_button,
        lv_color_black(),
        0
    );

    lv_obj_set_style_radius(
        start_button,
        4,
        0
    );

    // ==================================================
    // TEXT
    // ==================================================

    lv_obj_t *label =
        lv_label_create(
            start_button
        );

    lv_label_set_text(
        label,
        "START"
    );

    lv_obj_set_style_text_color(
        label,
        lv_color_white(),
        0
    );

    lv_obj_center(label);

    // ==================================================
    // CLICK
    // ==================================================

    lv_obj_add_event_cb(
        start_button,
        start_button_clicked,
        LV_EVENT_CLICKED,
        nullptr
    );
}


// ==================================================
// CAT ANIMATION
// ==================================================

static void animate_cat(
    lv_timer_t *timer
)
{
    (void)timer;

    // ==================================================
    // SLEEPING
    // ==================================================

    if (cat_state == CAT_SLEEPING)
    {
        current_frame++;

        if (current_frame >= 4)
        {
            current_frame = 0;
        }

        lv_image_set_src(
            cat,
            sleep_frames[current_frame]
        );

        return;
    }

    // ==================================================
    // STRETCHING
    // ==================================================

    if (cat_state == CAT_STRETCHING)
    {
        current_frame++;

        if (current_frame >= 10)
        {
            // --------------------------------------------------
            // Stretching finished.
            // --------------------------------------------------

            cat_state = CAT_SITTING;

            current_frame = 0;

            lv_image_set_src(
                cat,
                sitting_frames[0]
            );

            // --------------------------------------------------
            // Switch to sitting animation speed.
            // --------------------------------------------------

            lv_timer_set_period(
                animation_timer,
                SIT_SPEED
            );

            // --------------------------------------------------
            // START appears only when sitting.
            // --------------------------------------------------

            create_start_button();

            return;
        }

        lv_image_set_src(
            cat,
            stretching_frames[current_frame]
        );

        return;
    }

    // ==================================================
    // SITTING
    // ==================================================

    if (cat_state == CAT_SITTING)
    {
        current_frame++;

        if (current_frame >= 6)
        {
            current_frame = 0;
        }

        lv_image_set_src(
            cat,
            sitting_frames[current_frame]
        );
    }
}


// ==================================================
// CAT TOUCH
// ==================================================

static void cat_touched(
    lv_event_t *e
)
{
    (void)e;

    // ==================================================
    // SLEEPING -> STRETCHING
    // ==================================================

    if (cat_state == CAT_SLEEPING)
    {
        cat_state = CAT_STRETCHING;

        current_frame = 0;

        tap_count = 0;

        lv_image_set_src(
            cat,
            stretching_frames[0]
        );

        lv_timer_set_period(
            animation_timer,
            STRETCH_SPEED
        );

        return;
    }

    // ==================================================
    // IGNORE TOUCH DURING STRETCH
    // ==================================================

    if (cat_state == CAT_STRETCHING)
    {
        return;
    }

    // ==================================================
    // SITTING -> DOUBLE TAP -> SLEEPING
    // ==================================================

    if (cat_state == CAT_SITTING)
    {
        uint32_t now =
            lv_tick_get();

        // --------------------------------------------------
        // First tap
        // --------------------------------------------------

        if (tap_count == 0)
        {
            tap_count = 1;

            first_tap_time = now;

            return;
        }

        // --------------------------------------------------
        // Second tap
        // --------------------------------------------------

        if (tap_count == 1)
        {
            uint32_t elapsed =
                now - first_tap_time;

            if (elapsed <= DOUBLE_TAP_TIME)
            {
                // --------------------------------------------------
                // DOUBLE TAP DETECTED
                // --------------------------------------------------

                tap_count = 0;

                cat_state = CAT_SLEEPING;

                current_frame = 0;

                lv_image_set_src(
                    cat,
                    sleep_frames[0]
                );

                lv_timer_set_period(
                    animation_timer,
                    SLEEP_SPEED
                );

                // --------------------------------------------------
                // Remove START button.
                // --------------------------------------------------

                if (start_button != nullptr)
                {
                    lv_obj_delete(
                        start_button
                    );

                    start_button = nullptr;
                }

                return;
            }

            // --------------------------------------------------
            // Too slow.
            //
            // This tap becomes a new first tap.
            // --------------------------------------------------

            tap_count = 1;

            first_tap_time = now;
        }
    }
}


// ==================================================
// SHOW START SCREEN
// ==================================================

void start_screen_show()
{
    // ==================================================
    // BACKGROUND
    // ==================================================

    lv_obj_set_style_bg_color(
        lv_screen_active(),
        lv_palette_lighten(
            LV_PALETTE_GREY,
            1
        ),
        0
    );

    lv_obj_set_style_bg_opa(
        lv_screen_active(),
        LV_OPA_COVER,
        0
    );

    // ==================================================
    // RESET STATE
    // ==================================================

    cat_state = CAT_SLEEPING;

    current_frame = 0;

    tap_count = 0;

    first_tap_time = 0;

    cat = nullptr;

    start_button = nullptr;

    animation_timer = nullptr;

    // ==================================================
    // CREATE CAT
    // ==================================================

    cat =
        lv_image_create(
            lv_screen_active()
        );

    lv_image_set_src(
        cat,
        sleep_frames[0]
    );

    lv_obj_center(cat);

    lv_image_set_scale(
        cat,
        768
    );

    // ==================================================
    // CAT TOUCH AREA
    // ==================================================

    lv_obj_add_flag(
        cat,
        LV_OBJ_FLAG_CLICKABLE
    );

    lv_obj_add_event_cb(
        cat,
        cat_touched,
        LV_EVENT_RELEASED,
        nullptr
    );

    // ==================================================
    // ANIMATION TIMER
    // ==================================================

    animation_timer =
        lv_timer_create(
            animate_cat,
            SLEEP_SPEED,
            nullptr
        );
}


// ==================================================
// SET START BUTTON CALLBACK
// ==================================================

void start_screen_set_start_callback(
    StartButtonCallback callback
)
{
    start_callback = callback;
}


// ==================================================
// CLEANUP START SCREEN
// ==================================================

void start_screen_cleanup()
{
    // ==================================================
    // STOP ANIMATION TIMER
    // ==================================================

    if (animation_timer != nullptr)
    {
        lv_timer_delete(
            animation_timer
        );

        animation_timer = nullptr;
    }

    // ==================================================
    // DELETE START BUTTON
    // ==================================================

    if (start_button != nullptr)
    {
        lv_obj_delete(
            start_button
        );

        start_button = nullptr;
    }

    // ==================================================
    // DELETE CAT
    // ==================================================

    if (cat != nullptr)
    {
        lv_obj_delete(
            cat
        );

        cat = nullptr;
    }

    // ==================================================
    // RESET STATE
    // ==================================================

    cat_state = CAT_SLEEPING;

    current_frame = 0;

    tap_count = 0;

    first_tap_time = 0;
}