#ifndef BH1750_H
#define BH1750_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define BH1750_I2C_PORT i2c0 // SDA => 0 / SCL => 1

#define BH1750_ADDR 0x23
#define BH1750_POWER_ON 0x01
#define BH1750_CONT_HIGH_RES_MODE 0x10

void bh1750_init(void);
void bh1750_set_mode(uint8_t mode);
bool bh1750_read(float *lux);

#endif // BH1750_H