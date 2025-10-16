/**
 * @file main.cpp
 * @author Adrian Silva Palafox
 * @date 2024
 * @brief A simple web server on an ESP32 that displays a counter.
 * @details This program connects to a WiFi network, starts a web server, and displays a simple HTML page
 *          with a counter that increments every 250ms. It also blinks the built-in LED.
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "server.h" // This file should contain your WiFi credentials (ssid and password)

#define LED 2
#define DELAY_TIME 250

WebServer server(80);
uint8_t cont = 0;

/**
 * @brief Handles the root URL ("/").
 * @details Sends an HTML page with the current counter value to the client.
 */
void handleRoot()
{
  String html = "<html><body><h1>Count: " + String(cont) + "</h1></body></html>";
  server.send(200, "text/html", html);
}

/**
 * @brief Setup function, runs once at the beginning.
 */
void setup()
{
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Start the web server
  server.on("/", handleRoot);
  server.begin();
}

/**
 * @brief Main loop, runs repeatedly.
 */
void loop()
{
  // Blink the LED
  digitalWrite(LED, !digitalRead(LED));
  delay(DELAY_TIME);

  // Increment the counter and handle client requests
  cont++;
  server.handleClient();
}
