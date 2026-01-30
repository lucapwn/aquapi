#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include "pico/stdlib.h"

#include "drivers/ft232rl.h"
#include "drivers/aht10.h"
#include "drivers/bh1750.h"
#include "drivers/ssd1306.h"
#include "drivers/sd-card.h"
#include "drivers/moisture.h"
#include "drivers/relay.h"
#include "drivers/i2c.h"

#include "network/wifi.h"
#include "network/mqtt.h"

#include "services/freertos-tasks.h"

void setup(void);

#endif // MAIN_H