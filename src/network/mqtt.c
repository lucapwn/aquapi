#include "network/mqtt.h"

extern settings_t settings;
MQTT_CLIENT_T *client = NULL;

void mqtt_init(void)
{
    if (settings.mode != DEVICE_ONLINE_MODE)
    {
        return;
    }
    
    client = calloc(1, sizeof(MQTT_CLIENT_T));

    if (!client)
    {
        error_animation("Error: MQTT.");
        return;
    }

    client->connected = false;
    client->connecting = false;

    mqtt_connect();
}

void dns_found(const char *name, const ip_addr_t *ipaddr, void *callback)
{
    MQTT_CLIENT_T *c = (MQTT_CLIENT_T *)callback;

    if (!ipaddr)
    {
        error_animation("Error: DNS.");
        return;
    }

    c->remote_addr = *ipaddr;
}

void run_dns_lookup(void)
{
    if (dns_gethostbyname(MQTT_SERVER_HOST, &client->remote_addr, dns_found, client) == ERR_INPROGRESS)
    {
        while (ip_addr_isany(&client->remote_addr))
        {
            cyw43_arch_poll();
            sleep_ms(10);
        }
    }
}

void mqtt_connection_callback(mqtt_client_t *mqtt, void *arg, mqtt_connection_status_t status)
{
    MQTT_CLIENT_T *c = (MQTT_CLIENT_T *)arg;

    c->connecting = false;

    if (status != MQTT_CONNECT_ACCEPTED)
    {
        c->connected = false;
        c->reconnects++;
        error_animation("Error: MQTT.");
        return;
    }

    c->connected = true;

    mqtt_subscription(DEVICE_MESSAGE_MQTT_TOPIC, DEVICE_MESSAGE_MQTT_QOS);
}

err_t connects_to_mqtt_client(void)
{
    if (client->connecting || client->connected)
    {
        return ERR_OK;
    }

    struct mqtt_connect_client_info_t ci = { 0 };
    ci.client_id = "Raspberry Pi Pico W";
    client->connecting = true;

    return mqtt_client_connect(client->mqtt_client, &client->remote_addr, MQTT_SERVER_PORT, mqtt_connection_callback, client, &ci);
}

void mqtt_connect(void)
{
    ssd1306_clear_screen();
    ssd1306_write_text(0, 24, "Connecting to MQTT.");
    ssd1306_show_text();

    run_dns_lookup();

    if (!client->mqtt_client)
    {
        client->mqtt_client = mqtt_client_new();
    }

    if (!client->mqtt_client)
    {
        error_animation("Error: MQTT.");
        return;
    }

    connects_to_mqtt_client();
}

void mqtt_publish_start_callback(void *args, const char *topic, u32_t tot_len)
{
    MQTT_CLIENT_T *c = (MQTT_CLIENT_T *)args;

    memset(c->current_topic, 0, sizeof(c->current_topic));
    strncpy(c->current_topic, topic, sizeof(c->current_topic) - 1);

    c->payload_len = 0;
}

void mqtt_publish_data_callback(void *args, const u8_t *data, u16_t length, u8_t flags)
{
    MQTT_CLIENT_T *c = (MQTT_CLIENT_T *)args;

    if ((c->payload_len + length) >= MQTT_PAYLOAD_MAX_LEN)
    {
        return;
    }

    memcpy(&c->payload[c->payload_len], data, length);
    c->payload_len += length;

    if (flags & MQTT_DATA_FLAG_LAST)
    {
        c->payload[c->payload_len] = '\0';

        if (strcmp(c->current_topic, DEVICE_MESSAGE_MQTT_TOPIC) == 0)
        {
            load_json_settings(&settings, c->payload);
        }

        c->received++;
    }
}

void mqtt_subscription(const char *topic, u8_t qos)
{
    if (!client->connected)
    {
        return;
    }

    mqtt_set_inpub_callback(client->mqtt_client, mqtt_publish_start_callback, mqtt_publish_data_callback, client);

    if (mqtt_sub_unsub(client->mqtt_client, topic, qos, NULL, NULL, 1) != ERR_OK)
    {
        error_animation("Error: MQTT.");
    }
}

void mqtt_publishing(const char *topic, const void *payload, u16_t payload_length, u8_t qos)
{
    cyw43_arch_poll();

    if (!client->connected)
    {
        return;
    }

    if (mqtt_publish(client->mqtt_client, topic, payload, payload_length, qos, 0, NULL, client) != ERR_OK)
    {
        return;
    }

    client->published++;
    led_success_animation();
}
