#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#define MAX_TASKS 10
#define TASK_TITLE_LENGTH 64

struct Task
{
    char title[TASK_TITLE_LENGTH];

    bool completed;
};

// ==================================================
// INITIALIZE
// ==================================================

void task_manager_init();

// ==================================================
// GET
// ==================================================

int task_manager_get_count();

const Task *task_manager_get_task(
    int index
);

// ==================================================
// ADD
// ==================================================

void task_manager_add_task(
    const char *title
);

// ==================================================
// COMPLETE / UNCOMPLETE
// ==================================================

void task_manager_complete_task(
    int index
);

void task_manager_uncomplete_task(
    int index
);

// ==================================================
// CLEAR
// ==================================================

void task_manager_clear_tasks();

#endif