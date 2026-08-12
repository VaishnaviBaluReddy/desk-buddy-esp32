#include "app.h"

#include "lvgl.h"

#include "start_screen.h"
#include "task_screen.h"
#include "task_manager.h"
#include "network/wifi/wifi.h"

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
    // START WIFI
    // ==================================================

    wifi_init();

    // ==================================================
    // SHOW START SCREEN
    // ==================================================

    app_show_start_screen();
}

// ==================================================
// SHOW START SCREEN
// ==================================================

void app_show_start_screen()
{
    // --------------------------------------------------
    // Stop anything belonging to the previous
    // start screen before deleting its LVGL objects.
    // --------------------------------------------------

    start_screen_cleanup();

    // --------------------------------------------------
    // Remove all objects from the current screen.
    // --------------------------------------------------

    lv_obj_clean(
        lv_screen_active()
    );

    // --------------------------------------------------
    // Create the start screen again.
    // --------------------------------------------------

    start_screen_show();
}

// ==================================================
// SHOW TASK SCREEN
// ==================================================

void app_show_task_screen()
{
    // --------------------------------------------------
    // IMPORTANT:
    //
    // The start screen owns an animation timer.
    // lv_obj_clean() deletes the cat object, but it
    // does NOT automatically delete our custom timer.
    //
    // Therefore cleanup MUST happen BEFORE cleaning
    // the screen.
    // --------------------------------------------------

    start_screen_cleanup();

    // --------------------------------------------------
    // Remove the complete start screen.
    // --------------------------------------------------

    lv_obj_clean(
        lv_screen_active()
    );

    // --------------------------------------------------
    // Create task screen.
    // --------------------------------------------------

    task_screen_show();
}