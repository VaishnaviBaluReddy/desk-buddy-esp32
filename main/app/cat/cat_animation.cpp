#include "cat_animation.h"

#include "lvgl.h"

// ==================================================
// SITTING FRAMES
// ==================================================

#include "Assets/sit/sitting_0.h"
#include "Assets/sit/sitting_1.h"
#include "Assets/sit/sitting_2.h"
#include "Assets/sit/sitting_3.h"
#include "Assets/sit/sitting_4.h"
#include "Assets/sit/sitting_5.h"

// ==================================================
// ANIMATION SPEED
// ==================================================

#define SIT_SPEED 180

// ==================================================
// ANIMATION DATA
// ==================================================

struct CatAnimationData
{
    lv_obj_t *cat;

    uint8_t frame;

    lv_timer_t *timer;
};

// ==================================================
// GLOBAL TASK-SCREEN CAT
// ==================================================

static CatAnimationData *cat_data = nullptr;

// ==================================================
// FRAMES
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

// ==================================================
// TIMER CALLBACK
// ==================================================

static void cat_animation_timer(
    lv_timer_t *timer
)
{
    if (cat_data == nullptr)
    {
        return;
    }

    if (cat_data->cat == nullptr)
    {
        return;
    }

    // --------------------------------------------------
    // Advance frame
    // --------------------------------------------------

    cat_data->frame++;

    if (cat_data->frame >= 6)
    {
        cat_data->frame = 0;
    }

    lv_image_set_src(
        cat_data->cat,
        sitting_frames[
            cat_data->frame
        ]
    );
}

// ==================================================
// CREATE CAT
// ==================================================

lv_obj_t *cat_animation_create(
    lv_obj_t *parent,
    int scale
)
{
    // --------------------------------------------------
    // Remove previous animation if one somehow exists.
    // --------------------------------------------------

    cat_animation_destroy();

    // --------------------------------------------------
    // Allocate animation data.
    //
    // This is static/simple because there is currently
    // only one task-screen cat.
    // --------------------------------------------------

    static CatAnimationData data;

    cat_data = &data;

    cat_data->frame = 0;

    // --------------------------------------------------
    // Create image
    // --------------------------------------------------

    cat_data->cat =
        lv_image_create(parent);

    lv_image_set_src(
        cat_data->cat,
        sitting_frames[0]
    );

    // --------------------------------------------------
    // Scale
    // --------------------------------------------------

    lv_image_set_scale(
        cat_data->cat,
        scale
    );

    // --------------------------------------------------
    // Timer
    // --------------------------------------------------

    cat_data->timer =
        lv_timer_create(
            cat_animation_timer,
            SIT_SPEED,
            nullptr
        );

    return cat_data->cat;
}

// ==================================================
// BOTTOM RIGHT
// ==================================================

void cat_animation_set_bottom_right(
    lv_obj_t *cat,
    int x_offset,
    int y_offset
)
{
    if (cat == nullptr)
    {
        return;
    }

    lv_obj_align(
        cat,
        LV_ALIGN_BOTTOM_RIGHT,
        x_offset,
        y_offset
    );
}

// ==================================================
// DESTROY
// ==================================================

void cat_animation_destroy()
{
    if (cat_data == nullptr)
    {
        return;
    }

    // --------------------------------------------------
    // Delete timer FIRST.
    // --------------------------------------------------

    if (cat_data->timer != nullptr)
    {
        lv_timer_delete(
            cat_data->timer
        );

        cat_data->timer = nullptr;
    }

    // --------------------------------------------------
    // The LVGL image itself is removed by
    // lv_obj_clean() in app.cpp.
    // --------------------------------------------------

    cat_data->cat = nullptr;
    cat_data->frame = 0;

    cat_data = nullptr;
}