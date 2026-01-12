#include "drivers/bh1750.h"

void bh1750_init(void)
{
    bh1750_set_mode(BH1750_POWER_ON);
    sleep_ms(10);
    bh1750_set_mode(BH1750_CONT_HIGH_RES_MODE);
    sleep_ms(280);
}

void bh1750_set_mode(uint8_t mode)
{
    i2c_write_blocking(BH1750_I2C_PORT, BH1750_ADDR, &mode, 1, false);
}

bool bh1750_read(float *lux)
{
    uint8_t buffer[2];

    if (i2c_read_blocking(BH1750_I2C_PORT, BH1750_ADDR, buffer, 2, false) != 2)
    {
        return false; // Erro na leitura dos dados
    }
    
    uint16_t raw = (buffer[0] << 8) | buffer[1];
    *lux = raw / 1.2f;

    return true;
}
