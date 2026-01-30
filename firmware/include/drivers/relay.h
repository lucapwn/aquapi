#ifndef RELAY_H
#define RELAY_H

#include <stdio.h>
#include "pico/stdlib.h"

#include "app/manager.h"

#define RELAY_PIN 20

void relay_init(void);
void relay_on(void);
void relay_off(void);

#endif // RELAY_H