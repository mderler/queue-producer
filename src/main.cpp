#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <TCPClient.h>

#define QUEUE_SIZE 10

QueueHandle_t dataQueue;

// tcp-client config
const char* serverAddress = "your-esp32-ip-address";
const char* ssid = "your-ssid";
const char* password = "your-password";
const int serverPort = 80;
ESP_TCPClient tcpClient(serverAddress, serverPort);

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

    tcpClient.begin(ssid, password);

    xTaskCreate(readSensor, "Read Sensor", 1000, NULL, 1, NULL);
    xTaskCreate(sendData, "Send Data", 1000, NULL, 1, NULL);
}

void loop()
{
    tcpClient.sendMessage("Hello World!");
    delay(5000);
}
