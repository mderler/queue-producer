#include "TCPClient.h"

ESP_TCPClient::ESP_TCPClient(const char* serverAddress, int serverPort) : serverAddress(serverAddress), serverPort(serverPort) {}

void ESP_TCPClient::begin(const char* ssid, const char* password) {
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  client.connect(serverAddress, serverPort);
}

void ESP_TCPClient::reconnect() {
  if (!client.connected()) {
    client.connect(serverAddress, serverPort);
    if (client.connected()) {
      Serial.println("Connected to Server");
    } else {
      Serial.println("Connection to Server failed");
    }
  }
}

void ESP_TCPClient::sendMessage(const uint8_t message) {
  if (!client.connected()) {
    client.connect(serverAddress, serverPort);
    Serial.println("Connected to Server");
  }
  client.write(message);
  end();
}

void ESP_TCPClient::end() {
  client.stop();
  Serial.println("Disconnected from Server");
}