#include "task_manager.h"

#include <string.h>

static Task tasks[MAX_TASKS];

static int task_count = 0;


// ==================================================
// INITIALIZE
// ==================================================

void task_manager_init()
{
    task_count = 0;

    memset(tasks, 0, sizeof(tasks));
}


// ==================================================
// GET TASK COUNT
// ==================================================

int task_manager_get_count()
{
    return task_count;
}


// ==================================================
// GET TASK
// ==================================================

const Task* task_manager_get_task(int index)
{
    if (index < 0 || index >= task_count)
    {
        return nullptr;
    }

    return &tasks[index];
}


// ==================================================
// ADD TASK
// ==================================================

void task_manager_add_task(const char* title)
{
    if (task_count >= MAX_TASKS)
    {
        return;
    }

    strncpy(
        tasks[task_count].title,
        title,
        sizeof(tasks[task_count].title) - 1
    );

    tasks[task_count].title[
        sizeof(tasks[task_count].title) - 1
    ] = '\0';

    tasks[task_count].completed = false;

    task_count++;
}


// ==================================================
// COMPLETE TASK
// ==================================================

void task_manager_complete_task(int index)
{
    if (index < 0 || index >= task_count)
    {
        return;
    }

    tasks[index].completed = true;
}


// ==================================================
// CLEAR ALL TASKS
// ==================================================

void task_manager_clear_tasks()
{
    task_count = 0;

    memset(tasks, 0, sizeof(tasks));
}