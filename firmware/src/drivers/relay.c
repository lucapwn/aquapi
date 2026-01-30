#include "drivers/relay.h"

extern sensors_t sensors;

void relay_init(void)
{
    gpio_init(RELAY_PIN);
    gpio_set_dir(RELAY_PIN, GPIO_OUT);
    gpio_put(RELAY_PIN, 0);
}

void relay_on(void)
{
    sensors.relay = true;
    gpio_put(RELAY_PIN, sensors.relay);
}

void relay_off(void)
{
    sensors.relay = false;
    gpio_put(RELAY_PIN, sensors.relay);
}
