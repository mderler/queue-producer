#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <TCPServer.h>

#define QUEUE_SIZE 10

QueueHandle_t dataQueue;

const char *ssid = "HTL-Weiz";
const char *password = "HTL-Weiz";
const int serverPort = 1234;

void receiveData(void *parameters)
{
    ESP_TCPServer tcpServer(serverPort);

    String sensorData;
    for (;;)
    {
        tcpServer.begin(ssid, password);
        sensorData = tcpServer.handleClients();

        if (sensorData == "")
        {
            continue;
        }

        Serial.println("Received data: " + sensorData);

        if (xQueueSend(dataQueue, &sensorData, 0) == pdPASS)
        {
            Serial.print("Sent sensor data to queue: ");
            Serial.println(sensorData);
        }
        else
        {
            Serial.println("Error on sending to queue");
        }
    }
}

void displayData(void *parameters)
{
    String sensorData;
    for (;;)
    {
        vTaskDelay(500 / portTICK_PERIOD_MS);

        if (xQueueReceive(dataQueue, &sensorData, portMAX_DELAY) == pdTRUE)
        {
            Serial.print("Read sensor data from queue: ");
            Serial.println(sensorData);
        }
        else
        {
            Serial.println("Error on reading queue");
        }
    }
}

void setup()
{
    Serial.begin(9600);

    dataQueue = xQueueCreate(QUEUE_SIZE, 100);

    xTaskCreate(receiveData, "Receive Data", 10000, NULL, 1, NULL);
    xTaskCreate(displayData, "Display Data", 1000, NULL, 1, NULL);
}

void loop()
{
}
