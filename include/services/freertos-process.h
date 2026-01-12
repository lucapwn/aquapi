#ifndef FREERTOS_PROCESS_H
#define FREERTOS_PROCESS_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"

void vTaskProcess1(void *args);
void vTaskProcess2(void *args);
void freertos_process_init(void);

#endif // FREERTOS_PROCESS_H