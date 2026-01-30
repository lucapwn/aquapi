#ifndef MANAGER_H
#define MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#include "drivers/aht10.h"
#include "drivers/bh1750.h"
#include "drivers/ft232rl.h"
#include "drivers/moisture.h"
#include "drivers/relay.h"
#include "drivers/sd-card.h"
#include "drivers/ssd1306.h"
#include "drivers/led.h"
#include "drivers/settings.h"

#include "network/mqtt.h"

#define SENSORS_SLEEP_MS 800

typedef struct
{
    float temperature;
    float humidity;
    float lux;
    uint16_t moisture;
    bool relay;
} sensors_t;

void vSensorsTask(void *args);
void vSettingsTask(void *args);
void vPublishTask(void *args);
void vWateringTask(void *args);

#endif // MANAGER_H