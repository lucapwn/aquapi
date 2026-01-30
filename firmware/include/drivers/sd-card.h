#ifndef SD_CARD_H
#define SD_CARD_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "libraries/pico-sdcard/include/ff.h"
#include "drivers/ssd1306.h"

typedef enum
{
    SD_OK,
    SD_MOUNT_FAILED,
    SD_OPEN_FAILED,
    SD_WRITE_FAILED,
    SD_READ_FAILED,
    SD_CARD_NOT_INITIALIZED
} sd_status_t;

void sd_card_init(void);
DWORD get_fattime(void);
sd_status_t sd_card_write(const char *filename, const char *text);
sd_status_t sd_card_read(const char *filename, char *buffer, size_t buffer_size);

#endif // SD_CARD_H