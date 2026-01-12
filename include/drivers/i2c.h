#ifndef I2C_H
#define I2C_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define I2C_SENSOR_PORT i2c0

#define I2C_SENSOR_SDA_PIN 0
#define I2C_SENSOR_SCL_PIN 1

#define I2C_BAUD_RATE 400 * 1000 // 400 kHz

void i2c_bus_init(void);

#endif // I2C_H