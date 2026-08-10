#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <stdbool.h>

#define MAX_TASKS 10
#define TASK_TITLE_LENGTH 64

typedef struct
{
    char title[TASK_TITLE_LENGTH];
    bool completed;

} Task;

// Initialize task manager.
void task_manager_init();

// Return number of tasks.
int task_manager_get_count();

// Get task by index.
const Task *task_manager_get_task(int index);

// Add a task.
void task_manager_add_task(const char *title);

// Mark task as completed.
void task_manager_complete_task(int index);

// Toggle task completion.
void task_manager_toggle_task(int index);

// Clear all tasks.
void task_manager_clear_tasks();

#endif