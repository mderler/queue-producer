#include <Arduino.h>
#include <freertos/FreeRTOS.h>

#define QUEUE_SIZE 10

QueueHandle_t dataQueue;

void readSensor(void *parameters)
{
    for (;;)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void setup()
{
    dataQueue = xQueueCreate(QUEUE_SIZE, sizeof(int));

    xTaskCreate(readSensor, "Read Sensor", 1000, NULL, 1, NULL);
}

void loop()
{
}
