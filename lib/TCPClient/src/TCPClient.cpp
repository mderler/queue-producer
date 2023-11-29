#include "TCPClient.h"

ESP_TCPClient::ESP_TCPClient(const char* serverAddress, int serverPort) : serverAddress(serverAddress), serverPort(serverPort) {}

void ESP_TCPClient::begin() {
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Connect to the server
  client.connect(serverAddress, serverPort);
  if (client.connected()) {
    Serial.println("Connected to Server");
  } else {
    Serial.println("Connection to Server failed");
  }
}

void ESP_TCPClient::sendMessage(const String& message) {
  if (client.connected()) {
    // Send the message to the server
    client.print(message);
  } else {
    Serial.println("Client not connected");
  }
}

void ESP_TCPClient::end() {
  // Disconnect from the server
  client.stop();
  Serial.println("Disconnected from Server");
}