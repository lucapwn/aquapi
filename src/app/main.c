#include "app/main.h"

void setup(void)
{
    stdio_init_all();
    led_init();
    ssd1306_init();
    sd_card_init();
    settings_init();
    ft232rl_init();
    wifi_init();
    mqtt_init();
    i2c_bus_init();
    aht10_init();
    bh1750_init();
    moisture_init();
    relay_init();
}

int main(int argc, char *argv[])
{
    setup();
    freertos_tasks_init();

    return 0;
}
