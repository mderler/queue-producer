#include "TCPServer.h"

ESP_TCPServer::ESP_TCPServer(int port) : server(port) {}

void ESP_TCPServer::begin(const char *ssid, const char *password)
{

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");

    server.begin();
    Serial.println("Server started");
}

void ESP_TCPServer::handleClients()
{
    WiFiClient client = server.available();
    if (client)
    {
        Serial.println("New client connected");

        handleClient(client);
    }
}

void ESP_TCPServer::handleClient(WiFiClient client)
{

    client.println("Welcome to the ESP8266 TCP Server");

    String request = client.readStringUntil('\r');
    Serial.println("Client Request: " + request);

    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("");
    client.println("<html><body><h1>Hello from ESP8266!</h1></body></html>");

    client.stop();
    Serial.println("Client disconnected");
}