#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <TCPServer.h>

#define QUEUE_SIZE 10

QueueHandle_t dataQueue;

// tcp-server config
const char *ssid = "HTL-Weiz";
const char *password = "HTL-Weiz";
const int serverPort = 1234;

ESP_TCPServer tcpServer(serverPort);

void readSensor(void *parameters)
{
    int sensorData;
    for (;;)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        // read sensor here and store in sensorData variable

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

void sendData(void *parameters)
{
    int sensorData;
    for (;;)
    {
        vTaskDelay(500 / portTICK_PERIOD_MS);

        if (xQueueReceive(dataQueue, &sensorData, portMAX_DELAY) == pdTRUE)
        {
            Serial.print("Read sensor data from queue: ");
            Serial.println(sensorData);
            // send sensorData to consumer
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

    dataQueue = xQueueCreate(QUEUE_SIZE, sizeof(int));

    tcpServer.begin(ssid, password);

    // xTaskCreate(readSensor, "Read Sensor", 1000, NULL, 1, NULL);
    // xTaskCreate(sendData, "Send Data", 1000, NULL, 1, NULL);
}

void loop()
{
    tcpServer.handleClients();
}
