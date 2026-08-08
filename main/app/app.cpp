#include "app.h"

#include "lvgl.h"

#include "Assets/sprite_0.h"
#include "Assets/sprite_1.h"
#include "Assets/sprite_2.h"
#include "Assets/sprite_3.h"

static lv_obj_t* cat;

static const lv_image_dsc_t* frames[] =
{
    &sprite_0,
    &sprite_1,
    &sprite_2,
    &sprite_3
};

static uint8_t current_frame = 0;

static void animate_cat(lv_timer_t*)
{
    current_frame++;

    if(current_frame >= 4)
        current_frame = 0;

    lv_image_set_src(cat, frames[current_frame]);
}

void app_start()
{
    lv_obj_set_style_bg_color(
        lv_screen_active(),
        lv_palette_lighten(LV_PALETTE_GREY, 1),
        0);

    lv_obj_set_style_bg_opa(
        lv_screen_active(),
        LV_OPA_COVER,
        0);

    cat = lv_image_create(lv_screen_active());

    lv_image_set_src(cat, frames[0]);

    lv_obj_center(cat);

    lv_image_set_scale(cat, 512);

    lv_timer_create(animate_cat, 500, NULL);
}