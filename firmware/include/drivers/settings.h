#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pico/stdlib.h"

#include "drivers/ft232rl.h"
#include "drivers/sd-card.h"

#define DEVICE_SETTINGS_FILENAME "settings.txt"

#define DEFAULT_MODE_SETTING  1
#define DEFAULT_ON_SETTING    20
#define DEFAULT_OFF_SETTING   86
#define DEFAULT_STATE_SETTING 0

#define DEVICE_ONLINE_MODE  1
#define DEVICE_OFFLINE_MODE 0

#define WATERING_ALLOWED 1
#define WATERING_DENIED  0

typedef struct
{
    uint8_t mode;
    uint8_t on;
    uint8_t off;
    uint8_t state;
    uint8_t active;
} settings_t;

void settings_init(void);
void set_default_settings(settings_t *cfg);
void str_trim(char *str);
static bool parse_uint8_range(const char *str, uint8_t min, uint8_t max, uint8_t *out);
static const char *skip_spaces(const char *p);
bool load_json_settings(settings_t *settings, const char *json);
bool load_text_settings(const char *filename, settings_t *cfg);
bool settings_save(const char *filename, settings_t *cfg);
void settings_process_command(const char *cmd);
bool load_json_settings(settings_t *settings, const char *json);

#endif // SETTINGS_H