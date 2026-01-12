#include "services/freertos-process.h"

void vTaskProcess1(void *args)
{
    while (true)
    {
        printf("FreeRTOS: Processo [1]\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void vTaskProcess2(void *args)
{
    while (true)
    {
        printf("FreeRTOS: Processo [2]\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void freertos_process_init(void)
{
    xTaskCreate(vTaskProcess1, "vTaskProcess1", 128, NULL, 1, NULL);
    xTaskCreate(vTaskProcess2, "vTaskProcess2", 128, NULL, 1, NULL);

    vTaskStartScheduler();

    while (true)
    {
        tight_loop_contents();
    }
}
