#include "drivers/ft232rl.h"

void ft232rl_init(void)
{
    uart_init(FT232RL_UART_ID, FT232RL_BAUD_RATE);
    gpio_set_function(FT232RL_UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(FT232RL_UART_RX_PIN, GPIO_FUNC_UART);
}
