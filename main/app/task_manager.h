#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <stdbool.h>

// ==================================================
// CONFIGURATION
// ==================================================

#define MAX_TASKS 20
#define TASK_TITLE_LENGTH 64

// ==================================================
// TASK STRUCTURE
// ==================================================

typedef struct
{
    char title[TASK_TITLE_LENGTH];

    bool completed;

} Task;

// ==================================================
// INITIALIZE
// ==================================================

void task_manager_init();

// ==================================================
// GET TASK COUNT
// ==================================================

int task_manager_get_count();

// ==================================================
// GET TASK
// ==================================================

const Task *task_manager_get_task(
    int index
);

// ==================================================
// ADD TASK
// ==================================================

void task_manager_add_task(
    const char *title
);

// ==================================================
// COMPLETE TASK
// ==================================================

void task_manager_complete_task(
    int index
);

// ==================================================
// UNCOMPLETE TASK
// ==================================================

void task_manager_uncomplete_task(
    int index
);

// ==================================================
// CLEAR ALL TASKS
// ==================================================

void task_manager_clear_tasks();

#endif