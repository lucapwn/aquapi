#ifndef MOISTURE_H
#define MOISTURE_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

#define MOISTURE_ADC_PIN    28
#define MOISTURE_ADC_CHANNEL 2

#define MOISTURE_UPPER_LIMIT 3440
#define MOISTURE_LOWER_LIMIT 1260

void moisture_init(void);
uint16_t raw_to_percentage(uint16_t adc_value);

#endif // MOISTURE_H