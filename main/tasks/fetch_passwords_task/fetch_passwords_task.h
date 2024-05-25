#pragma once
#include "main.h"

#define FETCH_PASSWORDS_TASK_STACK_SIZE (int)(4 * 1024)
#define FETCH_PASSWORDS_TASK_PRIORITY 2

void start_fetch_passwords_task();
void finish_fetch_passwords_task();
