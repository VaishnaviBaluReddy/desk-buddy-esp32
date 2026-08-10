#include "app.h"

#include "lvgl.h"

#include "start_screen.h"
#include "task_manager.h"
#include "task_screen.h"

// ==================================================
// START BUTTON -> TASK SCREEN
// ==================================================

static void start_button_pressed()
{
    // --------------------------------------------------
    // Remove the entire start screen
    // --------------------------------------------------

    start_screen_cleanup();

    // --------------------------------------------------
    // Show task screen
    // --------------------------------------------------

    task_screen_show();
}


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
    // These will later come from the phone/app/backend.
    //

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
    // CONNECT START BUTTON TO APP ORCHESTRATION
    // ==================================================
    //
    // start_screen.cpp does NOT know about task_screen.
    //
    // It simply tells app.cpp:
    //
    // "START was pressed."
    //

    start_screen_set_start_callback(
        start_button_pressed
    );

    // ==================================================
    // SHOW START SCREEN
    // ==================================================

    start_screen_show();
}