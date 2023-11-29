#ifndef ESP_TCPSERVER_H
#define ESP_TCPSERVER_H

#include <WiFi.h>

class ESP_TCPServer
{
public:
    ESP_TCPServer(int port);

    void begin(const char *ssid, const char *password);
    String handleClients();

private:
    WiFiServer server;

    String handleClient(WiFiClient client);
};

#endif // ESP_TCPSERVER_H