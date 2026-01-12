#ifndef FT232RL_H
#define FT232RL_H

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"

#define FT232RL_UART_ID uart1
#define FT232RL_BAUD_RATE 115200

#define FT232RL_UART_TX_PIN 8
#define FT232RL_UART_RX_PIN 9

#define MAX_SERIAL_MESSAGE 64

void ft232rl_init(void);
void read_serial(void);

#endif // FT232RL_H