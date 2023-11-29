#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <Wifi.h>

#define QUEUE_SIZE 10

#define SSID ""
#define PASSWORD ""
#define HOST ""
#define PORT 1

QueueHandle_t dataQueue;

void connectToWifi()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        return;
    }

    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
}

void connectToHost(WiFiClient client)
{
    while (!client.connected())
    {
        if (client.connect(HOST, PORT))
        {
            Serial.println("Connected to server");
        }
        Serial.println("Connection failed");
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

void receiveData(void *parameters)
{
    WiFiClient client;
    int sensorData;
    for (;;)
    {

        connectToWifi();
        connectToHost(client);

        while (client.available() == 0)
        {
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }

        String response = client.readStringUntil('\r');
        Serial.println("Server response: " + response);

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

    xTaskCreate(receiveData, "Receive Data", 1000, NULL, 1, NULL);
    xTaskCreate(displayData, "Display Data", 1000, NULL, 1, NULL);
}

void loop()
{
}
