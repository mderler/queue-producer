#include "TCPHandler.h"

ESP_TCPServer::ESP_TCPServer(int port) : server(port) {}

void ESP_TCPServer::begin(const char* ssid, const char* password) {
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Start the server
  server.begin();
  Serial.println("Server started");
}

void ESP_TCPServer::handleClients() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client connected");
    // Handle the client
    handleClient(client);
  }
}

void ESP_TCPServer::handleClient(WiFiClient client) {
  // Send a welcome message to the client
  client.println("Welcome to the ESP8266 TCP Server");

  // Read the request from the client
  String request = client.readStringUntil('\r');
  Serial.println("Client Request: " + request);

  // Send a response to the client
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<html><body><h1>Hello from ESP8266!</h1></body></html>");

  // Close the connection
  client.stop();
  Serial.println("Client disconnected");
}