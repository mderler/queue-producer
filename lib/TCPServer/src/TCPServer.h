#ifndef ESP_TCPSERVER_H
#define ESP_TCPSERVER_H

#include <WiFi.h>

class ESP_TCPServer {
public:
  ESP_TCPServer(int port);

  void begin(const char* ssid, const char* password);
  void handleClients();

private:
  WiFiServer server;

  void handleClient(WiFiClient client);
};

#endif  // ESP_TCPSERVER_H