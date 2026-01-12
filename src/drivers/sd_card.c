#include "drivers/sd-card.h"

static FATFS fs;

void sd_card_init(void)
{   
    FRESULT fr;

    do {
        fr = f_mount(&fs, "", 1);

        if (fr != FR_OK) {
            printf("Failed to mount SD card (RESULT = %d)\n", fr);
            sleep_ms(1000);
        }
    } while (fr != FR_OK);

    printf("SD card successfully mounted.\n");
}

DWORD get_fattime(void)
{
    return ((DWORD)(2025 - 1980) << 25) | ((DWORD)7 << 21) | ((DWORD)9 << 16);
}

sd_status_t sd_card_write(const char *filename, const char *text)
{
    FIL fp;
    FRESULT fr = f_open(&fp, filename, FA_WRITE | FA_OPEN_ALWAYS | FA_OPEN_APPEND);

    if (fr != FR_OK)
    {
        printf("Error opening \"%s\" (FRESULT = %d).\n", filename, fr);
        return SD_OPEN_FAILED;
    }

    f_write(&fp, text, strlen(text), NULL);
    f_sync(&fp);
    f_close(&fp);
    
    printf("Text attached in \"%s\".\n", filename);
    return SD_OK;
}

sd_status_t sd_card_read(const char *filename, char *buffer, size_t buffer_size)
{
    FIL fp;
    UINT br;
    FRESULT fr = f_open(&fp, filename, FA_READ);

    memset(buffer, 0, buffer_size);

    if (fr != FR_OK)
    {
        printf("Failed to read \"%s\" (FRESULT = %d).\n", filename, fr);
        return SD_OPEN_FAILED;
    }

    f_read(&fp, buffer, buffer_size - 1, &br);
    f_close(&fp);
    
    return SD_OK;
}
