#include "app.h"

#include "lvgl.h"

#include "start_screen.h"
#include "task_screen.h"
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

    app_show_start_screen();
}

// ==================================================
// SHOW START SCREEN
// ==================================================

void app_show_start_screen()
{
    start_screen_show();
}

// ==================================================
// SHOW TASK SCREEN
// ==================================================

void app_show_task_screen()
{
    task_screen_show();
}