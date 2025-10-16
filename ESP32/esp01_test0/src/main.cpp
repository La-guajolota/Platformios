#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "server.h"

#define LED 2
#define DELAY_TIME 250

WebServer server(80);
uint8_t cont = 0;

void handleRoot()
{
  String html = "<html><body><h1>Count: " + String(cont) + "</h1></body></html>";
  server.send(200, "text/html", html);
}

void setup()
{
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.on("/", handleRoot);
  server.begin();
}

void loop()
{
  digitalWrite(LED, !digitalRead(LED));
  delay(DELAY_TIME);

  cont++;
  server.handleClient();
}