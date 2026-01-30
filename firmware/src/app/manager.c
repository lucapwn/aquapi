#include "app/manager.h"

bool sensorsValue = true;
bool settingsValue = true;
sensors_t sensors = { 0 };

QueueHandle_t sensorsQueue = NULL;
QueueHandle_t settingsQueue = NULL;

extern settings_t settings;
extern MQTT_CLIENT_T *client;

void vSensorsTask(void *args)
{
    char buffer[32];

    while (true)
    {
        if (!aht10_read(&sensors.temperature, &sensors.humidity))
        {
            error_animation("Error: AHT10.");
            continue;
        }

        if (!bh1750_read(&sensors.lux))
        {
            error_animation("Error: BH1750.");
            continue;
        }

        moisture_read(&sensors.moisture);

        ssd1306_clear_screen();
        snprintf(buffer, sizeof(buffer), "Temperature: %.2f C", sensors.temperature);
        ssd1306_write_text(0, 0, buffer);

        snprintf(buffer, sizeof(buffer), "Humidity:    %.2f%%", sensors.humidity);
        ssd1306_write_text(0, 8, buffer);

        snprintf(buffer, sizeof(buffer), "Brightness:  %.2f L", sensors.lux);
        ssd1306_write_text(0, 16, buffer);

        snprintf(buffer, sizeof(buffer), "Moisture:    %u%%", sensors.moisture);
        ssd1306_write_text(0, 24, buffer);

        snprintf(buffer, sizeof(buffer), "Mode:        %s", (settings.mode) ? "Online" : "Offline");
        ssd1306_write_text(0, 32, buffer);

        snprintf(buffer, sizeof(buffer), "Turn On:     %u%%", settings.on);
        ssd1306_write_text(0, 40, buffer);

        snprintf(buffer, sizeof(buffer), "Turn Off:    %u%%", settings.off);
        ssd1306_write_text(0, 48, buffer);

        snprintf(buffer, sizeof(buffer), "Relay:       %s", (sensors.relay) ? "On" : "Off");
        ssd1306_write_text(0, 56, buffer);
        ssd1306_show_text();

        xQueueSend(sensorsQueue, &sensorsValue, 0);
        vTaskDelay(pdMS_TO_TICKS(SENSORS_SLEEP_MS));
    }
}

void vSettingsTask(void *args)
{
    size_t index = 0;
    char uart_input[MAX_SERIAL_MESSAGE];

    while (true)
    {
        settingsValue = true;
        xQueueSend(settingsQueue, &settingsValue, 0);

        if (!uart_is_readable(FT232RL_UART_ID))
        {
            vTaskDelay(pdMS_TO_TICKS(10));
            continue;
        }

        uint8_t c = uart_getc(FT232RL_UART_ID);

        if (c >= 32 && c <= 126)
        {
            uart_input[index++] = c;
        }

        if (c == '\n')
        {
            uart_input[index] = '\0';
            settingsValue = false;
            xQueueSend(settingsQueue, &settingsValue, 0);
            settings_process_command(uart_input);
            index = 0;
        }
    }
}

void vPublishTask(void *args)
{
    char payload[MQTT_PAYLOAD_MAX_LEN];

    while (true)
    {
        if (!xQueueReceive(sensorsQueue, &sensorsValue, portMAX_DELAY))
        {
            vTaskDelay(pdMS_TO_TICKS(10));
            continue;
        }

        snprintf(payload, sizeof(payload),
            "{"
                "\"temperature\": %.2f,"
                "\"humidity\": %.2f,"
                "\"lux\": %.2f,"
                "\"moisture\": %u,"
                "\"relay\": %u"
            "}",
            sensors.temperature,
            sensors.humidity,
            sensors.lux,
            sensors.moisture,
            sensors.relay
        );

        mqtt_publishing(SERVER_MESSAGE_MQTT_TOPIC, payload, strlen(payload), SERVER_MESSAGE_MQTT_QOS);
    }
}

void vWateringTask(void *args)
{
    while (true)
    {
        if (!xQueueReceive(settingsQueue, &settingsValue, portMAX_DELAY) || !settingsValue)
        {
            vTaskDelay(pdMS_TO_TICKS(10));
            continue;
        }

        if (settings.mode == DEVICE_ONLINE_MODE)
        {
            // Aguarda receber o payload MQTT (retain) no tópico principal para carregar as informações da rega na variável de configurações
            if (strcmp(client->current_topic, DEVICE_MESSAGE_MQTT_TOPIC) != 0)
            {
                continue;
            }

            // Rega desativada na plataforma
            if (!settings.active)
            {
                continue;
            }
        }

        if (settings.on < 0 || settings.off > 100)
        {
            continue;
        }

        // Verifica se o dispositivo já completou o ciclo de rega (de 0% a 80%, por exemplo) e se pode iniciar novamente,
        // para garantir que o dispositivo não regue em um ciclo infinito, por exemplo:
        // Se o dispositivo chegar ao momento de desligar e a umidade do solo diminuir alguma porcentagem,
        // o dispositivo iria ficar ligado e iria deixar o solo no ponto máximo de umidade definido, e nunca iria chegar ao ponto mínimo de umidade do solo.
        if (sensors.moisture <= settings.on)
        {
            settings.state = WATERING_ALLOWED;
            settings_save(DEVICE_SETTINGS_FILENAME, &settings);
        }

        // Verifica se está no momento da rega iniciar e se ela está permitida
        if (sensors.moisture >= settings.on && settings.state == WATERING_ALLOWED)
        {
            relay_on();
        }

        // Verifica se está no momento da rega finalizar e garante que ela inicie somente após atingir a umidade mínima novamente
        if (sensors.moisture >= settings.off)
        {
            relay_off();
            settings.state = WATERING_DENIED;
            settings_save(DEVICE_SETTINGS_FILENAME, &settings);
        }

        vTaskDelay(pdMS_TO_TICKS(800));
    }
}
