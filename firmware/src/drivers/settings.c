#include "drivers/settings.h"

settings_t settings = { 0 };

// SET MODE=1
// SET ON=20
// SET OFF=84
// SAVE
// SHOW

void settings_init(void)
{
    settings_t tmp;

    if (load_text_settings(DEVICE_SETTINGS_FILENAME, &tmp))
    {
        settings = tmp;
    }
    else
    {
        set_default_settings(&settings);
        settings_save(DEVICE_SETTINGS_FILENAME, &settings);
    }

    if (settings.mode != DEVICE_ONLINE_MODE)
    {
        ssd1306_clear_screen();
        ssd1306_write_text(0, 24, "Offline mode enabled.");
        ssd1306_show_text();
        sleep_ms(2000);
        return;
    }

    ssd1306_clear_screen();
    ssd1306_write_text(0, 24, "Online mode enabled.");
    ssd1306_show_text();
    sleep_ms(2000);
}

void set_default_settings(settings_t *cfg)
{
    cfg->mode = DEFAULT_MODE_SETTING;
    cfg->on = DEFAULT_ON_SETTING;
    cfg->off = DEFAULT_OFF_SETTING;
    cfg->state = DEFAULT_STATE_SETTING;
}

void str_trim(char *str)
{
    char *end;

    while (*str == ' ' || *str == '\t')
    {
        str++;
    }

    end = str + strlen(str) - 1;

    while (end > str && (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n'))
    {
        *end-- = '\0';
    }
}

static bool parse_uint8_range(const char *str, uint8_t min, uint8_t max, uint8_t *out)
{
    char *end;
    long value = strtol(str, &end, 10);

    if (*end != '\0')
    {
        return false;
    }

    if (value < min || value > max)
    {
        return false;
    }

    *out = (uint8_t)value;
    return true;
}

static const char *skip_spaces(const char *p)
{
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
    {
        p++;
    }

    return p;
}

static bool parse_uint8_json_range(const char **p, uint8_t min, uint8_t max, uint8_t *out)
{
    int value = 0;
    const char *s = *p;

    if (*s < '0' || *s > '9')
    {
        return false;
    }

    while (*s >= '0' && *s <= '9')
    {
        value = value * 10 + (*s - '0');

        if (value > 255)
        {
            return false;
        }

        s++;
    }

    if (value < min || value > max)
    {
        return false;
    }

    *out = (uint8_t)value;
    *p = s;

    return true;
}

bool load_text_settings(const char *filename, settings_t *cfg)
{
    char buffer[512];

    if (sd_card_read(filename, buffer, sizeof(buffer)) != SD_OK)
    {
        return false;
    }

    bool has_mode = false;
    bool has_on = false;
    bool has_off = false;
    bool has_state = false;

    char *line = strtok(buffer, "\n");

    while (line)
    {
        str_trim(line);

        if (line[0] == '#' || line[0] == '\0')
        {
            line = strtok(NULL, "\n");
            continue;
        }

        char *eq = strchr(line, '=');

        if (!eq)
        {
            return false;
        }

        *eq = '\0';
        char *key = line;
        char *value = eq + 1;

        str_trim(key);
        str_trim(value);

        uint8_t v;

        if (strcmp(key, "MODE") == 0)
        {
            if (!parse_uint8_range(value, 0, 1, &v))
            {
                return false;
            }

            cfg->mode = v;
            has_mode = true;
        }
        else if (strcmp(key, "ON") == 0)
        {
            if (!parse_uint8_range(value, 0, 100, &v))
            {
                return false;
            }

            cfg->on = v;
            has_on = true;
        }
        else if (strcmp(key, "OFF") == 0)
        {
            if (!parse_uint8_range(value, 0, 100, &v))
            {
                return false;
            }

            cfg->off = v;
            has_off = true;
        }
        else if (strcmp(key, "STATE") == 0)
        {
            if (!parse_uint8_range(value, 0, 1, &v))
            {
                return false;
            }

            cfg->state = v;
            has_state = true;
        }
        else
        {
            return false;
        }

        line = strtok(NULL, "\n");
    }

    if (!has_mode || !has_on || !has_off || !has_state)
    {
        return false;
    }

    if (cfg->on >= cfg->off)
    {
        return false;
    }

    return true;
}

bool settings_save(const char *filename, settings_t *cfg)
{
    char text[256];

    snprintf(text, sizeof(text),
        "# Device configuration mode (1 = Online, 0 = Offline)\n"
        "MODE=%u\n\n"
        "# Soil moisture ranges (0%% - 100%%)\n"
        "ON=%u\n"
        "OFF=%u\n\n"
        "# Last system status (DO NOT CHANGE)\n"
        "STATE=%u\n",
        cfg->mode,
        cfg->on,
        cfg->off,
        cfg->state
    );

    if (sd_card_write(filename, text) != SD_OK)
    {
        return false;
    }

    return true;
}

void settings_process_command(const char *cmd)
{
    if (strcmp(cmd, "SHOW") == 0)
    {
        char msg[128];

        snprintf(msg, sizeof(msg),
            "MODE=%u\nON=%u\nOFF=%u\nSTATE=%u\n",
            settings.mode,
            settings.on,
            settings.off,
            settings.state
        );
        
        uart_puts(FT232RL_UART_ID, msg);
        return;
    }

    if (strcmp(cmd, "SAVE") == 0)
    {
        if (settings_save(DEVICE_SETTINGS_FILENAME, &settings))
        {
            uart_puts(FT232RL_UART_ID, "Settings saved successfully.\n");
        }
        else
        {
            uart_puts(FT232RL_UART_ID, "Error saving settings.\n");
        }

        return;
    }

    if (strncmp(cmd, "SET ", 4) == 0)
    {
        char key[16];
        char value[16];

        if (sscanf(cmd + 4, "%15[^=]=%15s", key, value) != 2)
        {
            uart_puts(FT232RL_UART_ID, "Invalid format.\n");
            return;
        }

        uint8_t tmp;

        if (strcmp(key, "MODE") == 0 && parse_uint8_range(value, 0, 1, &tmp))
        {
            settings.mode = tmp;
        }
        else if (strcmp(key, "ON") == 0 && parse_uint8_range(value, 0, 100, &tmp))
        {
            settings.on = tmp;
        }
        else if (strcmp(key, "OFF") == 0 && parse_uint8_range(value, 0, 100, &tmp))
        {
            settings.off = tmp;
        }
        else
        {
            uart_puts(FT232RL_UART_ID, "Invalid key or value.\n");
            return;
        }

        if (settings.on >= settings.off)
        {
            uart_puts(FT232RL_UART_ID, "ON must be less than OFF.\n");
            return;
        }

        uart_puts(FT232RL_UART_ID, "Value successfully set in memory.\nDon't forget to save the information.\n");
    }
}

bool load_json_settings(settings_t *settings, const char *json)
{
    char key[16];
    const char *p = json;

    if (!settings || !json)
    {
        return false;
    }

    p = skip_spaces(p);

    if (*p != '{')
    {
        return false;
    }

    p++;

    while (*p)
    {
        p = skip_spaces(p);

        if (*p == '}')
        {
            return true;
        }

        if (*p != '"')
        {
            return false;
        }

        p++;
        size_t i = 0;

        while (*p && *p != '"' && i < sizeof(key) - 1)
        {
            key[i++] = *p++;
        }

        key[i] = '\0';

        if (*p != '"')
        {
            return false;
        }

        p++;
        p = skip_spaces(p);

        if (*p != ':')
        {
            return false;
        }

        p++;
        p = skip_spaces(p);
        uint8_t value;

        if (strcmp(key, "on") == 0 && parse_uint8_json_range(&p, 0, 100, &value))
        {
            settings->on = value;
        }
        else if (strcmp(key, "off") == 0 && parse_uint8_json_range(&p, 0, 100, &value))
        {
            settings->off = value;
        }
        else if (strcmp(key, "active") == 0 && parse_uint8_json_range(&p, 0, 1, &value))
        {
            settings->active = value;
        }
        else
        {
            return false;
        }

        p = skip_spaces(p);

        if (*p == ',')
        {
            p++;
            continue;
        }
        else if (*p == '}')
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    return false;
}
