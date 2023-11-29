#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <freertos/FreeRTOS.h>
#include <Adafruit_I2CDevice.h>
#include <SPI.h>

Adafruit_BMP280 bmp;

void readSensor()
{
    if (!bmp.begin(0x76))
    {
        Serial.println("Could not find a valid BMP280 sensor :/");
        return;
    }

    for (;;)
    {
        // Read sensor data
        float temperature = bmp.readTemperature();

        // Print sensor data
        Serial.print("Temperature = ");
        Serial.print(temperature);
        Serial.println(" *C");

        delay(1000);
    }
}

void setup()
{
    delay(1000);
    Serial.begin(9600);
    Wire.begin();
}

void loop()
{
    readSensor();
}
