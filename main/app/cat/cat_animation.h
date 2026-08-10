#ifndef CAT_ANIMATION_H
#define CAT_ANIMATION_H

#include "lvgl.h"

// ==================================================
// CREATE TASK-SCREEN CAT
// ==================================================
//
// Creates a sitting animated cat on the given parent.
//
// scale:
//     265 for the current task-screen cat
//
// The returned object is the LVGL image object.
//
// ==================================================

lv_obj_t *cat_animation_create(
    lv_obj_t *parent,
    int32_t scale
);

// ==================================================
// POSITION CAT
// ==================================================
//
// Convenience function for changing the cat position.
//
// ==================================================

void cat_animation_set_bottom_right(
    lv_obj_t *cat,
    int32_t x_offset,
    int32_t y_offset
);

#endif