#include "network/wifi.h"

extern settings_t settings;

void wifi_init(void)
{
    if (settings.mode != DEVICE_ONLINE_MODE)
    {
        return;
    }

    ssd1306_clear_screen();
    ssd1306_write_text(0, 24, "Connecting to Wi-Fi.");
    ssd1306_show_text();

    while (cyw43_arch_init())
    {
        error_animation("Error: Wi-Fi.");
    }

    cyw43_arch_enable_sta_mode();

    while (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWORD, CYW43_AUTH_WPA2_AES_PSK, 10000) != 0)
    {
        error_animation("Error: Wi-Fi.");
    }
}
