#ifndef MQTT_H
#define MQTT_H

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "lwip/dns.h"
#include "lwip/tcp.h"
#include "lwip/apps/mqtt.h"
#include "lwip/altcp_tls.h"
#include "lwip/altcp_tcp.h"
#include "drivers/ssd1306.h"
#include "drivers/led.h"
#include "drivers/settings.h"

// broker.hivemq.com -> https://www.hivemq.com/demos/websocket-client/

#define MQTT_SERVER_HOST "broker.hivemq.com"
#define MQTT_SERVER_PORT 1883

#define SERVER_MESSAGE_MQTT_TOPIC "message/server"
#define DEVICE_MESSAGE_MQTT_TOPIC "message/device"

#define MQTT_TOPIC_MAX_LEN   128
#define MQTT_PAYLOAD_MAX_LEN 512

#define SERVER_MESSAGE_MQTT_QOS 2
#define DEVICE_MESSAGE_MQTT_QOS 2

#define PUBLISH_DELAY_MS 1000

typedef struct MQTT_CLIENT_T_
{
    ip_addr_t remote_addr;
    mqtt_client_t *mqtt_client;
    bool connected;
    bool connecting;
    u32_t received;
    u32_t published;
    u32_t reconnects;
    char current_topic[MQTT_TOPIC_MAX_LEN];
    char payload[MQTT_PAYLOAD_MAX_LEN];
    u16_t payload_len;
} MQTT_CLIENT_T;

void mqtt_init(void);
void mqtt_connect(void);
void mqtt_subscription(const char *topic, u8_t qos);
void mqtt_publishing(const char *topic, const void *payload, u16_t payload_length, u8_t qos);
err_t connects_to_mqtt_client(void);
void run_dns_lookup(void);
void dns_found(const char *name, const ip_addr_t *ipaddr, void *callback);
void mqtt_connection_callback(mqtt_client_t *client, void *arg, mqtt_connection_status_t status);
void mqtt_publish_start_callback(void *args, const char *topic, u32_t tot_len);
void mqtt_publish_data_callback(void *args, const u8_t *data, u16_t length, u8_t flags);

#endif // MQTT_H