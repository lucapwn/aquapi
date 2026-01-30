#include "drivers/ssd1306.h"

ssd1306_t display;

void ssd1306_init(void)
{
    i2c_init(SSD1306_I2C_PORT, I2C_BAUD_RATE);

    gpio_set_function(SSD1306_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SSD1306_SCL_PIN, GPIO_FUNC_I2C);

    gpio_pull_up(SSD1306_SDA_PIN);
    gpio_pull_up(SSD1306_SCL_PIN);

    display.external_vcc = false;

    ssd1306_initialize(&display, SSD1306_WIDTH, SSD1306_HEIGHT, SSD1306_I2C_ADDRESS, SSD1306_I2C_PORT);
    ssd1306_clear(&display);
}

void ssd1306_write_text(uint32_t x, uint32_t y, const char *text)
{
    ssd1306_draw_string(&display, x, y, SSD1306_TEXT_SCALE, text);
}

void ssd1306_show_text(void)
{
    ssd1306_show(&display);
}

void ssd1306_clear_screen(void)
{
    ssd1306_clear(&display);
}

void error_animation(const char *text)
{
    ssd1306_clear_screen();
    ssd1306_write_text(0, 24, text);
    ssd1306_show_text();
    led_error_animation();
}
