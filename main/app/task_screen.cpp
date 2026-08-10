#include "task_screen.h"

#include "lvgl.h"

#include <stdint.h>

#include "task_manager.h"
#include "cat/cat_animation.h"

// ==================================================
// TASK CHECKBOX CALLBACK
// ==================================================

static void task_checkbox_clicked(
    lv_event_t *e
)
{
    // ==================================================
    // GET CHECKBOX
    // ==================================================

    lv_obj_t *checkbox =
        static_cast<lv_obj_t *>(
            lv_event_get_target(e)
        );

    // ==================================================
    // GET TASK INDEX
    // ==================================================

    int index =
        static_cast<int>(
            reinterpret_cast<intptr_t>(
                lv_event_get_user_data(e)
            )
        );

    // ==================================================
    // GET CHECKED STATE
    // ==================================================

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
        task_manager_complete_task(
            index
        );
    }
    else
    {
        task_manager_uncomplete_task(
            index
        );
    }

    // ==================================================
    // STRIKETHROUGH
    // ==================================================

    if (checked)
    {
        lv_obj_set_style_text_decor(
            checkbox,
            LV_TEXT_DECOR_STRIKETHROUGH,
            LV_PART_MAIN
        );
    }
    else
    {
        lv_obj_set_style_text_decor(
            checkbox,
            LV_TEXT_DECOR_NONE,
            LV_PART_MAIN
        );
    }
}


// ==================================================
// TASK SCREEN
// ==================================================

void task_screen_show()
{
    // ==================================================
    // CREATE A COMPLETELY NEW SCREEN
    // ==================================================
    //
    // IMPORTANT:
    //
    // Do NOT use:
    //
    //     lv_screen_active()
    //
    // here.
    //
    // We want a separate LVGL screen from the start
    // screen.
    //

    lv_obj_t *screen =
        lv_obj_create(nullptr);

    // ==================================================
    // SCREEN BACKGROUND
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
    // TITLE
    // ==================================================

    lv_obj_t *title =
        lv_label_create(
            screen
        );

    lv_label_set_text(
        title,
        "TASKS"
    );

    lv_obj_set_style_text_color(
        title,
        lv_color_black(),
        0
    );

    lv_obj_set_style_text_font(
        title,
        LV_FONT_DEFAULT,
        0
    );

    lv_obj_align(
        title,
        LV_ALIGN_TOP_LEFT,
        30,
        20
    );

    // ==================================================
    // TASK LIST
    // ==================================================

    int task_count =
        task_manager_get_count();

    int y_position = 70;

    for (
        int i = 0;
        i < task_count;
        i++
    )
    {
        // ==================================================
        // GET TASK
        // ==================================================

        const Task *task =
            task_manager_get_task(
                i
            );

        if (task == nullptr)
        {
            continue;
        }

        // ==================================================
        // TASK ROW
        // ==================================================

        lv_obj_t *row =
            lv_obj_create(
                screen
            );

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

        // ==================================================
        // ROW BACKGROUND
        // ==================================================

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

        // ==================================================
        // ROW BORDER
        // ==================================================

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

        // ==================================================
        // ROW CORNERS
        // ==================================================

        lv_obj_set_style_radius(
            row,
            0,
            0
        );

        // ==================================================
        // REMOVE INTERNAL PADDING
        // ==================================================

        lv_obj_set_style_pad_all(
            row,
            0,
            0
        );

        // ==================================================
        // CHECKBOX
        // ==================================================

        lv_obj_t *checkbox =
            lv_checkbox_create(
                row
            );

        lv_checkbox_set_text(
            checkbox,
            task->title
        );

        // ==================================================
        // TEXT COLOR
        // ==================================================

        lv_obj_set_style_text_color(
            checkbox,
            lv_color_black(),
            LV_PART_MAIN
        );

        // ==================================================
        // CHECKBOX POSITION
        // ==================================================

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
            // --------------------------------------------------
            // Checked
            // --------------------------------------------------

            lv_obj_add_state(
                checkbox,
                LV_STATE_CHECKED
            );

            // --------------------------------------------------
            // Strike-through
            // --------------------------------------------------

            lv_obj_set_style_text_decor(
                checkbox,
                LV_TEXT_DECOR_STRIKETHROUGH,
                LV_PART_MAIN
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

        // ==================================================
        // NEXT TASK
        // ==================================================

        y_position += 60;
    }

    // ==================================================
    // TASK-SCREEN CAT
    // ==================================================

    lv_obj_t *cat =
        cat_animation_create(
            screen,
            265
        );

    cat_animation_set_bottom_right(
        cat,
        -10,
        -10
    );

    // ==================================================
    // NOW LOAD THE NEW SCREEN
    // ==================================================

    lv_screen_load(
        screen
    );
}