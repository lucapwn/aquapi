#include "drivers/relay.h"

void relay_init(void)
{
    gpio_init(RELAY_PIN);
    gpio_set_dir(RELAY_PIN, GPIO_OUT);
    gpio_put(RELAY_PIN, 0);
}

void relay_on(void)
{
    gpio_put(RELAY_PIN, 1);
    printf("Relay [ON]\n");
}

void relay_off(void)
{
    gpio_put(RELAY_PIN, 0);
    printf("Relay [OFF]\n");
}
