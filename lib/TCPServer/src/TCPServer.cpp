#include "TCPServer.h"

ESP_TCPServer::ESP_TCPServer(int port) : server(port) {}

void ESP_TCPServer::begin(const char *ssid, const char *password)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        return;
    }

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    Serial.println(WiFi.localIP());

    server.begin();
    Serial.println("Server started");
}

String ESP_TCPServer::handleClients()
{
    WiFiClient client = server.available();
    if (client)
    {
        return handleClient(client);
    }
    return "";
}

String ESP_TCPServer::handleClient(WiFiClient client)
{
    if (client.connected() && client.available())
    {
        String request = client.readStringUntil('\r');
        return request;
    }
    return "";
}