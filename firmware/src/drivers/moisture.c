#include "drivers/moisture.h"

void moisture_init(void)
{
    adc_init();
    adc_gpio_init(MOISTURE_ADC_PIN);
    adc_select_input(MOISTURE_ADC_CHANNEL);
}

void moisture_read(uint16_t *moisture)
{
    uint16_t adc_value = adc_read();
    *moisture = raw_to_percentage(adc_value);
}

uint16_t raw_to_percentage(uint16_t adc_value)
{
    if (adc_value > MOISTURE_UPPER_LIMIT)
    {
        adc_value = MOISTURE_UPPER_LIMIT;
    }

    if (adc_value < MOISTURE_LOWER_LIMIT)
    {
        adc_value = MOISTURE_LOWER_LIMIT;
    }

    uint32_t range = MOISTURE_UPPER_LIMIT - MOISTURE_LOWER_LIMIT;
    uint32_t difference = MOISTURE_UPPER_LIMIT - adc_value;

    return (difference * 100) / range;
}
