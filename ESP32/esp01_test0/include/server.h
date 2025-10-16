#ifndef SERVER_H
#define SERVER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char *ssid = ""; // Replace with your network SSID
const char *password = "";   // Replace with your network password

extern WebServer server;

void startServer();
void handleRoot();
void handleNotFound();

#endif // SERVER_H