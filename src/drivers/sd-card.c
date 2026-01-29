#include "drivers/sd-card.h"

static FATFS fs;

void sd_card_init(void)
{   
    FRESULT fr;

    do
    {
        fr = f_mount(&fs, "", 1);

        if (fr != FR_OK)
        {
            error_animation("Error: SD Card.");
        }
    }
    while (fr != FR_OK);
}

DWORD get_fattime(void)
{
    return ((DWORD)(2026 - 1980) << 25) | ((DWORD)1 << 21) | ((DWORD)8 << 16);
}

sd_status_t sd_card_write(const char *filename, const char *text)
{
    FIL fp;

    if (f_open(&fp, filename, FA_WRITE | FA_CREATE_ALWAYS) != FR_OK)
    {
        error_animation("Error: SD Card.");
        return SD_OPEN_FAILED;
    }

    f_write(&fp, text, strlen(text), NULL);
    f_sync(&fp);
    f_close(&fp);

    return SD_OK;
}

sd_status_t sd_card_read(const char *filename, char *buffer, size_t buffer_size)
{
    FIL fp;
    UINT br;
    FRESULT fr = f_open(&fp, filename, FA_READ);

    if (fr != FR_OK)
    {
        error_animation("Error: SD Card.");
        return SD_OPEN_FAILED;
    }

    memset(buffer, 0, buffer_size);

    fr = f_read(&fp, buffer, buffer_size - 1, &br);
    f_close(&fp);

    if (fr != FR_OK || br == 0)
    {
        error_animation("Error: SD Card.");
        return SD_READ_FAILED;
    }

    buffer[br] = '\0';
    return SD_OK;
}
