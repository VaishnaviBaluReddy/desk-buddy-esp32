#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <stdint.h>

#define MAX_TASKS 10

struct Task
{
    char title[64];
    bool completed;
};

void task_manager_init();

int task_manager_get_count();

const Task* task_manager_get_task(int index);

void task_manager_add_task(const char* title);

void task_manager_complete_task(int index);

void task_manager_clear_tasks();

#endif