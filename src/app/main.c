#include "app/main.h"

void setup(void)
{
    stdio_init_all();
    wifi_init();
    ft232rl_init();
    i2c_bus_init();
    aht10_init();
    bh1750_init();
    moisture_init();
    ssd1306_init();
    sd_card_init();
    relay_init();
}

void read_sensors_and_modules(void)
{
    float temperature = 0.0f;
    float humidity = 0.0f;
    float lux = 0.0f;

    uint16_t adc_value = 0;
    uint16_t moisture = 0;

    const char *filename = "hello.txt";

    char buffer_write[512];
    char buffer_read[512];

    sd_status_t read_result = 0;

    while (true)
    {
        ssd1306_clear_screen();
        ssd1306_print_text(0, 8, "Hello, World!");

        if (aht10_read(&temperature, &humidity))
        {
            printf("Temperature: %.2f °C\nHumidity: %.2f%%\n", temperature, humidity);
        }
        else
        {
            printf("Error to read AHT10 sensor.\n");
        }

        if (bh1750_read(&lux))
        {
            printf("Lighting: %.2f lux\n", lux);
        }
        else
        {
            printf("Error to read BH1750 sensor.\n");
        }

        adc_value = adc_read();
        moisture = raw_to_percentage(adc_value);

        printf("Soil Moisture: %u%%\n", moisture);

        memset(buffer_write, 0, sizeof(buffer_write));
        memset(buffer_read, 0, sizeof(buffer_read));

        snprintf(buffer_write, sizeof(buffer_write), "%s", "Hello, World!\n");
        sd_card_write(filename, buffer_write);

        read_result = sd_card_read(filename, buffer_read, sizeof(buffer_read));

        if (read_result == SD_OK)
        {
            printf("Content of \"%s\":\n%s\n", filename, buffer_read);
        }
        else
        {
            printf("Error reading content from \"%s\".\n", filename);
        }

        relay_on();
        sleep_ms(1000);

        relay_off();
        sleep_ms(2000);
    }
}

int main(int argc, char *argv[])
{
    setup();

    read_sensors_and_modules();
    // freertos_process_init();
    // read_serial();

    // MQTT_CLIENT_T *client = mqtt_client_init();

    // create_mqtt_client(client);
    // start_mqtt_connection(client);
    // start_mqtt_subscription(client);

    // while (true)
    // {
    //     start_mqtt_publishing(client);
    // }

    // cyw43_arch_deinit();

    return 0;
}
