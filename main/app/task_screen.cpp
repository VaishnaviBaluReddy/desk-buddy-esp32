#include "task_screen.h"

#include "lvgl.h"

#include "task_manager.h"
#include "cat/cat_animation.h"
#include "header/header.h"

// ==================================================
// TASK CHECKBOX CALLBACK
// ==================================================

static void task_checkbox_clicked(
    lv_event_t *e
)
{
    lv_obj_t *checkbox =
        static_cast<lv_obj_t *>(
            lv_event_get_target(e)
        );

    int index =
        static_cast<int>(
            reinterpret_cast<intptr_t>(
                lv_event_get_user_data(e)
            )
        );

    bool checked =
        lv_obj_has_state(
            checkbox,
            LV_STATE_CHECKED
        );

    // ==================================================
    // UPDATE TASK MANAGER
    // ==================================================

    if (checked)
    {
        task_manager_complete_task(index);
    }
    else
    {
        task_manager_uncomplete_task(index);
    }

    // ==================================================
    // STRIKETHROUGH
    // ==================================================

    if (checked)
    {
        lv_obj_set_style_text_decor(
            checkbox,
            LV_TEXT_DECOR_STRIKETHROUGH,
            0
        );

        // Make completed text lighter.
        lv_obj_set_style_text_color(
            checkbox,
            lv_color_hex(0x888888),
            0
        );
    }
    else
    {
        lv_obj_set_style_text_decor(
            checkbox,
            LV_TEXT_DECOR_NONE,
            0
        );

        // Restore normal text.
        lv_obj_set_style_text_color(
            checkbox,
            lv_color_black(),
            0
        );
    }
}

// ==================================================
// TASK SCREEN
// ==================================================

void task_screen_show()
{
    lv_obj_t *screen =
        lv_screen_active();

    // ==================================================
    // BACKGROUND
    // ==================================================

    lv_obj_set_style_bg_color(
        screen,
        lv_color_white(),
        0
    );

    lv_obj_set_style_bg_opa(
        screen,
        LV_OPA_COVER,
        0
    );

    // ==================================================
    // HEADER
    // ==================================================

    header_create(screen);

    // ==================================================
    // TASK LIST
    // ==================================================

    int task_count =
        task_manager_get_count();

    int y_position = 95;

    for (int i = 0; i < task_count; i++)
    {
        const Task *task =
            task_manager_get_task(i);

        if (task == nullptr)
        {
            continue;
        }

        // ==================================================
        // TASK ROW
        // ==================================================

        lv_obj_t *row =
            lv_obj_create(screen);

        lv_obj_set_size(
            row,
            500,
            50
        );

        lv_obj_set_pos(
            row,
            30,
            y_position
        );

        lv_obj_set_style_bg_color(
            row,
            lv_color_white(),
            0
        );

        lv_obj_set_style_bg_opa(
            row,
            LV_OPA_COVER,
            0
        );

        lv_obj_set_style_border_color(
            row,
            lv_color_black(),
            0
        );

        lv_obj_set_style_border_width(
            row,
            1,
            0
        );

        lv_obj_set_style_radius(
            row,
            0,
            0
        );

        lv_obj_set_style_pad_all(
            row,
            0,
            0
        );

        // ==================================================
        // CHECKBOX
        // ==================================================

        lv_obj_t *checkbox =
            lv_checkbox_create(row);

        lv_checkbox_set_text(
            checkbox,
            task->title
        );

        lv_obj_set_style_text_color(
            checkbox,
            task->completed
                ? lv_color_hex(0x888888)
                : lv_color_black(),
            0
        );

        lv_obj_align(
            checkbox,
            LV_ALIGN_LEFT_MID,
            10,
            0
        );

        // ==================================================
        // EXISTING COMPLETED STATE
        // ==================================================

        if (task->completed)
        {
            lv_obj_add_state(
                checkbox,
                LV_STATE_CHECKED
            );

            lv_obj_set_style_text_decor(
                checkbox,
                LV_TEXT_DECOR_STRIKETHROUGH,
                0
            );
        }

        // ==================================================
        // CLICK EVENT
        // ==================================================

        lv_obj_add_event_cb(
            checkbox,
            task_checkbox_clicked,
            LV_EVENT_CLICKED,
            reinterpret_cast<void *>(
                static_cast<intptr_t>(i)
            )
        );

        y_position += 60;
    }

    // ==================================================
    // TASK SCREEN CAT
    // ==================================================

    lv_obj_t *cat =
        cat_animation_create(
            screen,
            384
        );

    cat_animation_set_bottom_right(
        cat,
        -10,
        -10
    );
}