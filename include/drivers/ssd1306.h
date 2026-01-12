#ifndef SSD1306_H
#define SSD1306_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"

#include "libraries/pico-ssd1306/ssd1306.h"

#define SSD1306_I2C_PORT i2c1

#define SSD1306_SDA_PIN 14
#define SSD1306_SCL_PIN 15

#define I2C_BAUD_RATE 400 * 1000 // 400 kHz

#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64

#define SSD1306_I2C_ADDRESS 0x3C
#define SSD1306_TEXT_SCALE 1

void ssd1306_init(void);
void ssd1306_print_text(uint32_t x, uint32_t y, const char *text);
void ssd1306_clear_screen(void);

#endif // SSD1306_H