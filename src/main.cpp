#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <TCPClient.h>
#include <Adafruit_I2CDevice.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#define QUEUE_SIZE 10

QueueHandle_t dataQueue;

Adafruit_BMP280 bmp;

// tcp-client config
const char *serverAddress = "172.31.183.143";
const char *ssid = "HTL-Weiz";
const char *password = "HTL-Weiz";
const int serverPort = 1234;

void readSensor()
{
    if (!bmp.begin(0x76))
    {
        Serial.println("Could not find a valid BMP280 sensor :/");
        return;
    }

    for (;;)
    {
        vTaskDelay(500 / portTICK_PERIOD_MS);
        // Read sensor data
        float temperature = bmp.readTemperature();
        if (xQueueSend(dataQueue, &temperature, portMAX_DELAY) == pdTRUE)
        {
            Serial.print("Send sensor data from queue: ");
            Serial.println(temperature);
            // send sensorData to consumer
        }
        else
        {
            Serial.println("Error on sending queue");
        }
    }
}

void sendData(void *parameters)
{
    float sensorData;
    ESP_TCPClient tcpClient(serverAddress, serverPort);
    tcpClient.begin(ssid, password);
    for (;;)
    {
        vTaskDelay(500 / portTICK_PERIOD_MS);

        if (xQueueReceive(dataQueue, &sensorData, portMAX_DELAY) == pdTRUE)
        {
            Serial.print("Read sensor data from queue: ");
            Serial.println(sensorData);
            // send sensorData to consumer
            tcpClient.sendMessage(String(sensorData));
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

    dataQueue = xQueueCreate(QUEUE_SIZE, sizeof(float));

    // (readSensor, "Read Sensor", 1000, NULL, 1, NULL);
    // xTaskCreate(sendData, "Send Data", 1000, NULL, 1, NULL);
}

void loop()
{
}
