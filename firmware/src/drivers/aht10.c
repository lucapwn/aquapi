#include "drivers/aht10.h"

void aht10_init(void)
{
    uint8_t payload[] = { 0xBE, 0x08, 0x00 };
    i2c_write_blocking(AHT10_I2C_PORT, AHT10_ADDR, payload, 3, false);
    sleep_ms(250);
}

bool aht10_read(float *temperature, float *humidity)
{
    uint8_t data[6];
    uint8_t payload[] = { 0xAC, 0x33, 0x00 };

    i2c_write_blocking(AHT10_I2C_PORT, AHT10_ADDR, payload, 3, false);
    sleep_ms(280);

    if (i2c_read_blocking(AHT10_I2C_PORT, AHT10_ADDR, data, 6, false) != 6)
    {
        return false;
    }

    if (data[0] & 0x80)
    {
        return false;
    }

    uint32_t raw_humidity = (data[1] << 12) | (data[2] << 4) | ((data[3] >> 4) & 0x0F);
    uint32_t raw_temperature = ((data[3] & 0x0F) << 16) | (data[4] << 8) | data[5];

    *humidity = (raw_humidity / 1048576.0f) * 100.0f;
    *temperature = (raw_temperature / 1048576.0f) * 200.0f - 50.0f;
    
    return true;
}
