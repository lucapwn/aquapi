#include "services/freertos-tasks.h"

extern settings_t settings;
extern QueueHandle_t sensorsQueue;
extern QueueHandle_t settingsQueue;

void freertos_tasks_init(void)
{
    sensorsQueue = xQueueCreate(1, sizeof(bool));
    settingsQueue = xQueueCreate(1, sizeof(bool));

    // Ajustar o tamanho de words quando finalizar todas as tasks
    xTaskCreate(vSensorsTask, "vSensorsTask", 512, NULL, 1, NULL);
    xTaskCreate(vSettingsTask, "vSettingsTask", 512, NULL, 1, NULL);
    xTaskCreate(vWateringTask, "vWateringTask", 512, NULL, 1, NULL);

    if (settings.mode == DEVICE_ONLINE_MODE)
    {
        xTaskCreate(vPublishTask, "vPublishTask", 512, NULL, 1, NULL);
    }

    vTaskStartScheduler();

    while (true)
    {
        tight_loop_contents();
    }

    cyw43_arch_deinit();
}
