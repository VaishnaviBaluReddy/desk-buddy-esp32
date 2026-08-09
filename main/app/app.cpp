#include "app.h"
#include "lvgl.h"
#include "task_manager.h"

// --------------------------------------------------
// SLEEPING ANIMATION
// --------------------------------------------------

#include "Assets/sleep/sprite_0.h"
#include "Assets/sleep/sprite_1.h"
#include "Assets/sleep/sprite_2.h"
#include "Assets/sleep/sprite_3.h"

// --------------------------------------------------
// SITTING ANIMATION
// --------------------------------------------------

#include "Assets/sit/sitting_0.h"
#include "Assets/sit/sitting_1.h"
#include "Assets/sit/sitting_2.h"
#include "Assets/sit/sitting_3.h"
#include "Assets/sit/sitting_4.h"
#include "Assets/sit/sitting_5.h"

// --------------------------------------------------
// STRETCHING ANIMATION
// --------------------------------------------------

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
// ANIMATION SPEEDS
// ==================================================

#define SLEEP_SPEED       500
#define STRETCH_SPEED     350
#define SIT_SPEED         180


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
// CAT OBJECT
// ==================================================

static lv_obj_t* cat = nullptr;


// ==================================================
// START BUTTON
// ==================================================

static lv_obj_t* start_button = nullptr;


// ==================================================
// ANIMATION FRAMES
// ==================================================

static const lv_image_dsc_t* sleep_frames[] =
{
    &sprite_0,
    &sprite_1,
    &sprite_2,
    &sprite_3
};

static const lv_image_dsc_t* stretching_frames[] =
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

static const lv_image_dsc_t* sitting_frames[] =
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

static lv_timer_t* animation_timer = nullptr;


// ==================================================
// DOUBLE TAP VARIABLES
// ==================================================

static uint8_t tap_count = 0;

static uint32_t first_tap_time = 0;

static const uint32_t DOUBLE_TAP_TIME = 500;


// ==================================================
// START BUTTON -> BLANK PAGE
// ==================================================

static void start_button_pressed(lv_event_t* e)
{
    (void)e;

    // Stop the animation timer
    if (animation_timer != nullptr)
    {
        lv_timer_pause(animation_timer);
    }

    // Remove everything from the current screen
    lv_obj_clean(lv_screen_active());

    // Blank page for now.
}


// ==================================================
// CREATE START BUTTON
// ==================================================

static void create_start_button()
{
    // Don't create it twice
    if (start_button != nullptr)
        return;


    // --------------------------------------------------
    // Create button
    // --------------------------------------------------

    start_button = lv_button_create(
        lv_screen_active()
    );


    // --------------------------------------------------
    // Button size
    // --------------------------------------------------

    lv_obj_set_size(
        start_button,
        180,
        55
    );


    // --------------------------------------------------
    // Position
    //
    // Sitting cat is centered.
    // Move button below it.
    // --------------------------------------------------

    lv_obj_align(
        start_button,
        LV_ALIGN_CENTER,
        0,
        150
    );


    // --------------------------------------------------
    // BLACK PIXEL-STYLE APPEARANCE
    // --------------------------------------------------

    lv_obj_set_style_bg_color(
        start_button,
        lv_color_black(),
        0
    );

    lv_obj_set_style_bg_opa(
        start_button,
        LV_OPA_COVER,
        0
    );


    // Sharp square corners
    lv_obj_set_style_radius(
        start_button,
        0,
        0
    );


    // No border
    lv_obj_set_style_border_width(
        start_button,
        0,
        0
    );


    // No shadow
    lv_obj_set_style_shadow_width(
        start_button,
        0,
        0
    );


    // --------------------------------------------------
    // Button text
    // --------------------------------------------------

    lv_obj_t* label = lv_label_create(
        start_button
    );

    lv_label_set_text(
        label,
        "START"
    );


    // White text
    lv_obj_set_style_text_color(
        label,
        lv_color_white(),
        0
    );


    // Center text
    lv_obj_center(label);


    // --------------------------------------------------
    // Button touch
    // --------------------------------------------------

    lv_obj_add_event_cb(
        start_button,
        start_button_pressed,
        LV_EVENT_CLICKED,
        NULL
    );
}


// ==================================================
// ANIMATION TIMER
// ==================================================

static void animate_cat(lv_timer_t* timer)
{
    (void)timer;


    switch (cat_state)
    {
        // ------------------------------------------
        // SLEEPING
        // ------------------------------------------

        case CAT_SLEEPING:

            current_frame++;

            if (current_frame >= 4)
            {
                current_frame = 0;
            }

            lv_image_set_src(
                cat,
                sleep_frames[current_frame]
            );

            break;


        // ------------------------------------------
        // STRETCHING
        // ------------------------------------------

        case CAT_STRETCHING:

            current_frame++;

            if (current_frame >= 10)
            {
                // Stretching is finished.

                cat_state = CAT_SITTING;

                current_frame = 0;

                lv_image_set_src(
                    cat,
                    sitting_frames[0]
                );


                // Change animation speed
                // to sitting speed.

                lv_timer_set_period(
                    animation_timer,
                    SIT_SPEED
                );


                // Show START button.

                create_start_button();

                return;
            }


            lv_image_set_src(
                cat,
                stretching_frames[current_frame]
            );

            break;


        // ------------------------------------------
        // SITTING
        // ------------------------------------------

        case CAT_SITTING:

            current_frame++;

            if (current_frame >= 6)
            {
                current_frame = 0;
            }

            lv_image_set_src(
                cat,
                sitting_frames[current_frame]
            );

            break;
    }
}


// ==================================================
// CAT TOUCH
// ==================================================

static void cat_touched(lv_event_t* e)
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


        // Make sure button isn't present.

        if (start_button != nullptr)
        {
            lv_obj_delete(start_button);

            start_button = nullptr;
        }


        // Start stretching.

        lv_image_set_src(
            cat,
            stretching_frames[0]
        );


        // Stretching speed.

        lv_timer_set_period(
            animation_timer,
            STRETCH_SPEED
        );


        // Make sure timer is running.

        lv_timer_resume(
            animation_timer
        );

        return;
    }


    // ==================================================
    // IGNORE TOUCH DURING STRETCHING
    // ==================================================

    if (cat_state == CAT_STRETCHING)
    {
        return;
    }


    // ==================================================
    // SITTING -> DOUBLE TAP
    // ==================================================

    if (cat_state == CAT_SITTING)
    {
        uint32_t now = lv_tick_get();


        // ----------------------------------------------
        // FIRST TAP
        // ----------------------------------------------

        if (tap_count == 0)
        {
            tap_count = 1;

            first_tap_time = now;

            return;
        }


        // ----------------------------------------------
        // SECOND TAP
        // ----------------------------------------------

        if (tap_count == 1)
        {
            uint32_t elapsed =
                now - first_tap_time;


            // ------------------------------------------
            // DOUBLE TAP
            // ------------------------------------------

            if (elapsed <= DOUBLE_TAP_TIME)
            {
                tap_count = 0;


                // Remove START button.

                if (start_button != nullptr)
                {
                    lv_obj_delete(start_button);

                    start_button = nullptr;
                }


                // Go back to sleeping.

                cat_state = CAT_SLEEPING;

                current_frame = 0;


                lv_image_set_src(
                    cat,
                    sleep_frames[0]
                );


                // Sleeping speed.

                lv_timer_set_period(
                    animation_timer,
                    SLEEP_SPEED
                );


                // Make sure timer is running.

                lv_timer_resume(
                    animation_timer
                );

                return;
            }


            // ------------------------------------------
            // TOO SLOW
            //
            // Treat current tap as a new first tap.
            // ------------------------------------------

            tap_count = 1;

            first_tap_time = now;

            return;
        }
    }
}


// ==================================================
// START APPLICATION
// ==================================================

void app_start()
{
    // ------------------------------------------------
    // Background
    // ------------------------------------------------

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


    // ------------------------------------------------
    // Create cat
    // ------------------------------------------------

    cat = lv_image_create(
        lv_screen_active()
    );


    // ------------------------------------------------
    // Start sleeping
    // ------------------------------------------------

    lv_image_set_src(
        cat,
        sleep_frames[0]
    );


    // ------------------------------------------------
    // Center cat
    // ------------------------------------------------

    lv_obj_center(cat);


    // ------------------------------------------------
    // Scale image
    // ------------------------------------------------

    lv_image_set_scale(
        cat,
        768
    );


    // ------------------------------------------------
    // Make cat itself touchable
    //
    // The clickable area follows the image object.
    // ------------------------------------------------

    lv_obj_add_flag(
        cat,
        LV_OBJ_FLAG_CLICKABLE
    );


    // ------------------------------------------------
    // Cat touch event
    // ------------------------------------------------

    lv_obj_add_event_cb(
        cat,
        cat_touched,
        LV_EVENT_RELEASED,
        NULL
    );


    // ------------------------------------------------
    // Animation timer
    // ------------------------------------------------

    animation_timer = lv_timer_create(
        animate_cat,
        SLEEP_SPEED,
        NULL
    );

    task_manager_init();

    task_manager_add_task("Finish Desk Buddy");
    task_manager_add_task("Go for a run");
    task_manager_add_task("Study AWS");
}