#include "drivers/ft232rl.h"

void ft232rl_init(void)
{
    uart_init(FT232RL_UART_ID, FT232RL_BAUD_RATE);
    gpio_set_function(FT232RL_UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(FT232RL_UART_RX_PIN, GPIO_FUNC_UART);
}

void read_serial(void)
{
    size_t index = 0;
    char received[MAX_SERIAL_MESSAGE];

    while (true)
    {
        if (uart_is_readable(FT232RL_UART_ID))
        {
            uint8_t c = uart_getc(FT232RL_UART_ID);

            // Guarda no buffer se não for controle
            if (c >= 32 && c <= 126)
            {
                received[index++] = c;
            }

            // Colocar LF no "Line ending" do Serial Monitor para a aplicação enviar o "\n" ao final da string
            if (c == '\n')
            {
                received[index] = '\0';
                uart_puts(FT232RL_UART_ID, "Received: ");
                uart_puts(FT232RL_UART_ID, received);
                uart_puts(FT232RL_UART_ID, "\n");

                if (strcmp(received, "admin") == 0)
                {
                    uart_puts(FT232RL_UART_ID, "Lucas is my owner! :)\n");
                }

                index = 0;
            }
        }
    }
}
