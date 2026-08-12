#ifndef CAT_ANIMATION_H
#define CAT_ANIMATION_H

#include "lvgl.h"

// ==================================================
// CREATE CAT
// ==================================================

lv_obj_t *cat_animation_create(
    lv_obj_t *parent,
    int scale
);

// ==================================================
// POSITION CAT
// ==================================================

void cat_animation_set_bottom_right(
    lv_obj_t *cat,
    int x_offset,
    int y_offset
);

// ==================================================
// DESTROY CAT ANIMATION
// ==================================================
//
// Stops its animation timer before the object is
// removed from the screen.
//

void cat_animation_destroy();

#endif