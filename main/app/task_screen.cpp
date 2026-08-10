#include "task_screen.h"

#include "lvgl.h"

#include "task_manager.h"
#include "cat/cat_animation.h"
#include "header/header.h"

// ==================================================
// TASK CHECKBOX CALLBACK
// ==================================================

static void task_checkbox_clicked(lv_event_t *e)
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
    // CHECK CURRENT STATE
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
        task_manager_complete_task(index);
    }
    else
    {
        task_manager_uncomplete_task(index);
    }

    // ==================================================
    // COMPLETED TASK STYLE
    // ==================================================

    if (checked)
    {
        // Lighter text

        lv_obj_set_style_text_color(
            checkbox,
            lv_color_hex(0x888888),
            0
        );

        // Strikethrough

        lv_obj_set_style_text_decor(
            checkbox,
            LV_TEXT_DECOR_STRIKETHROUGH,
            0
        );
    }
    else
    {
        // Normal text

        lv_obj_set_style_text_color(
            checkbox,
            lv_color_black(),
            0
        );

        // Remove strikethrough

        lv_obj_set_style_text_decor(
            checkbox,
            LV_TEXT_DECOR_NONE,
            0
        );
    }
}

// ==================================================
// TASK SCREEN
// ==================================================

void task_screen_show()
{
    // ==================================================
    // GET CURRENT SCREEN
    // ==================================================

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
    // TITLE
    // ==================================================

    lv_obj_t *title =
        lv_label_create(screen);

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

    // Header occupies roughly the first 60 px.
    // No separator line.

    lv_obj_align(
        title,
        LV_ALIGN_TOP_LEFT,
        30,
        75
    );

    // ==================================================
    // TASK LIST
    // ==================================================

    int task_count =
        task_manager_get_count();

    // Start below header.

    int y_position = 115;

    for (int i = 0; i < task_count; i++)
    {
        // ==================================================
        // GET TASK
        // ==================================================

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

        // --------------------------------------------------
        // Row background
        // --------------------------------------------------

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

        // --------------------------------------------------
        // Row border
        // --------------------------------------------------

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

        // --------------------------------------------------
        // Row corners
        // --------------------------------------------------

        lv_obj_set_style_radius(
            row,
            0,
            0
        );

        // --------------------------------------------------
        // Remove internal padding
        // --------------------------------------------------

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

        // --------------------------------------------------
        // Checkbox position
        // --------------------------------------------------

        lv_obj_align(
            checkbox,
            LV_ALIGN_LEFT_MID,
            10,
            0
        );

        // ==================================================
        // TASK STYLE
        // ==================================================

        if (task->completed)
        {
            lv_obj_add_state(
                checkbox,
                LV_STATE_CHECKED
            );

            lv_obj_set_style_text_color(
                checkbox,
                lv_color_hex(0x888888),
                0
            );

            lv_obj_set_style_text_decor(
                checkbox,
                LV_TEXT_DECOR_STRIKETHROUGH,
                0
            );
        }
        else
        {
            lv_obj_set_style_text_color(
                checkbox,
                lv_color_black(),
                0
            );

            lv_obj_set_style_text_decor(
                checkbox,
                LV_TEXT_DECOR_NONE,
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
            384
        );

    cat_animation_set_bottom_right(
        cat,
        -10,
        -10
    );
}

// ==================================================
// CLEANUP
// ==================================================

void task_screen_cleanup()
{
    // --------------------------------------------------
    // Nothing special yet.
    //
    // The task screen currently consists of children
    // of the active screen.
    //
    // When we move to the next screen, the active
    // screen will be recreated/cleared by the
    // navigation layer.
    // --------------------------------------------------
}