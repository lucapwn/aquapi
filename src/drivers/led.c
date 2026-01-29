#include "drivers/led.h"

void led_init(void)
{
    gpio_init(RED_LED_PIN);
    gpio_init(GREEN_LED_PIN);

    gpio_set_dir(RED_LED_PIN, GPIO_OUT);
    gpio_set_dir(GREEN_LED_PIN, GPIO_OUT);
}

void led_error_animation(void)
{
    for (int i = 0; i < 2; i++)
    {
        gpio_put(RED_LED_PIN, 1);
        sleep_ms(60);
        gpio_put(RED_LED_PIN, 0);
        sleep_ms(60);
    }

    sleep_ms(760);
}

void led_success_animation(void)
{
    gpio_put(GREEN_LED_PIN, 1);
    sleep_ms(40);

    gpio_put(GREEN_LED_PIN, 0);
    sleep_ms(40);
}
