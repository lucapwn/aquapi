#include "drivers/i2c.h"

void i2c_bus_init(void)
{
    i2c_init(I2C_SENSOR_PORT, I2C_BAUD_RATE);

    gpio_set_function(I2C_SENSOR_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SENSOR_SCL_PIN, GPIO_FUNC_I2C);

    gpio_pull_up(I2C_SENSOR_SDA_PIN);
    gpio_pull_up(I2C_SENSOR_SCL_PIN);
}
