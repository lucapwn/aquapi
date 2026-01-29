#ifndef WIFI_H
#define WIFI_H

#include <stdio.h>
#include "lwip/netif.h"
#include "lwip/ip4_addr.h"
#include "pico/cyw43_arch.h"
#include "drivers/ssd1306.h"
#include "drivers/led.h"
#include "drivers/settings.h"

#define WIFI_SSID     // "YOUR_WIFI_NAME_HERE"
#define WIFI_PASSWORD // "YOUR_WIFI_PASSWORD_HERE"

void wifi_init(void);

#endif // WIFI_H