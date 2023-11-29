#ifndef ESP_TCPCLIENT_H
#define ESP_TCPCLIENT_H

#include <WiFi.h>

class ESP_TCPClient
{
public:
  ESP_TCPClient(const char *serverAddress, int serverPort);

  void begin(const char *ssid, const char *password);
  void reconnect();
  void sendMessage(const String &message);
  void end();

private:
  WiFiClient client;
  const char *serverAddress;
  int serverPort;
  const char *ssid = "your-ssid";
  const char *password = "your-password";
};

#endif