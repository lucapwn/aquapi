#ifndef FREERTOS_PROCESS_H
#define FREERTOS_PROCESS_H

#include <stdio.h>
#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include "task.h"

#include "app/manager.h"
#include "drivers/settings.h"

void freertos_tasks_init(void);

#endif // FREERTOS_PROCESS_H