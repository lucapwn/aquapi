#ifndef LED_H
#define LED_H

#include <stdio.h>
#include "pico/stdlib.h"

#define RED_LED_PIN   13
#define GREEN_LED_PIN 11

void led_init(void);
void led_error_animation(void);
void led_success_animation(void);

#endif // LED_H