#ifndef AHT10_H
#define AHT10_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define AHT10_ADDR 0x38
#define AHT10_I2C_PORT i2c0 // SDA => 0 / SCL => 1

void aht10_init(void);
bool aht10_read(float *temperature, float *humidity);

#endif // AHT10_H