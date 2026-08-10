#include "cat_animation.h"

#include "lvgl.h"

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
// SETTINGS
// ==================================================

#define CAT_SIT_SPEED 180

// ==================================================
// SITTING FRAMES
// ==================================================

static const lv_image_dsc_t *sitting_frames[] =
{
    &sitting_0,
    &sitting_1,
    &sitting_2,
    &sitting_3,
    &sitting_4,
    &sitting_5
};

static const uint8_t SITTING_FRAME_COUNT = 6;

// ==================================================
// CAT VARIABLES
// ==================================================

static lv_obj_t *cat = nullptr;

static lv_timer_t *animation_timer = nullptr;

static uint8_t current_frame = 0;

// ==================================================
// ANIMATION TIMER
// ==================================================

static void cat_animation_timer(lv_timer_t *timer)
{
    (void)timer;

    // Safety check
    if (cat == nullptr)
    {
        return;
    }

    current_frame++;

    if (current_frame >= SITTING_FRAME_COUNT)
    {
        current_frame = 0;
    }

    lv_image_set_src(
        cat,
        sitting_frames[current_frame]
    );
}

// ==================================================
// CREATE CAT
// ==================================================

lv_obj_t *cat_animation_create(
    lv_obj_t *parent,
    int32_t scale
)
{
    // --------------------------------------------------
    // Reset animation state
    // --------------------------------------------------

    current_frame = 0;

    // --------------------------------------------------
    // Create image
    // --------------------------------------------------

    cat = lv_image_create(parent);

    // --------------------------------------------------
    // First frame
    // --------------------------------------------------

    lv_image_set_src(
        cat,
        sitting_frames[0]
    );

    // --------------------------------------------------
    // Scale
    // --------------------------------------------------

    lv_image_set_scale(
        cat,
        scale
    );

    // --------------------------------------------------
    // Bottom-right position
    // --------------------------------------------------

    lv_obj_align(
        cat,
        LV_ALIGN_BOTTOM_RIGHT,
        -10,
        -10
    );

    // --------------------------------------------------
    // Create animation timer
    // --------------------------------------------------

    animation_timer =
        lv_timer_create(
            cat_animation_timer,
            CAT_SIT_SPEED,
            nullptr
        );

    return cat;
}

// ==================================================
// SET BOTTOM-RIGHT POSITION
// ==================================================

void cat_animation_set_bottom_right(
    lv_obj_t *cat_object,
    int32_t x_offset,
    int32_t y_offset
)
{
    if (cat_object == nullptr)
    {
        return;
    }

    lv_obj_align(
        cat_object,
        LV_ALIGN_BOTTOM_RIGHT,
        x_offset,
        y_offset
    );
}