#include "app.h"

#include "lvgl.h"

#include "start_screen.h"
#include "task_manager.h"

// ==================================================
// START APPLICATION
// ==================================================

void app_start()
{
    // ==================================================
    // INITIALIZE TASK MANAGER
    // ==================================================

    task_manager_init();

    // ==================================================
    // TEMPORARY TEST TASKS
    // ==================================================
    //
    // These are only here so the task manager
    // already contains some data.
    //
    // Later:
    // WiFi -> app/backend -> task manager
    //
    // ==================================================

    task_manager_add_task(
        "Finish Desk Buddy"
    );

    task_manager_add_task(
        "Workout"
    );

    task_manager_add_task(
        "Study"
    );

    task_manager_add_task(
        "Read"
    );

    // ==================================================
    // SHOW START SCREEN
    // ==================================================

    start_screen_show();
}

// ==================================================
// SHOW TASK PAGE
// ==================================================
//
// TEMPORARY.
//
// This is intentionally just a blank page for now.
// Later this function will call:
//
//     task_screen_show();
//
// ==================================================

void app_show_task_page()
{
    // Remove everything currently on screen.

    lv_obj_clean(
        lv_screen_active()
    );

    // --------------------------------------------------
    // Temporary blank task page
    // --------------------------------------------------

    lv_obj_set_style_bg_color(
        lv_screen_active(),
        lv_color_white(),
        0
    );

    lv_obj_set_style_bg_opa(
        lv_screen_active(),
        LV_OPA_COVER,
        0
    );
}