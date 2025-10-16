/**
 * @file main.cpp
 * @author Adrian Silva Palafox
 * @date 2024
 * @brief A simple program to send messages over two UART ports on an ESP32.
 * @details This program sends a message to Serial (UART0) and Serial2 every second.
 */

#include <Arduino.h>

unsigned long lastUART = 0;
const unsigned long intervalUART = 1000; // 1 second
 
/**
 * @brief Setup function, runs once at the beginning.
 */
void setup() {
  Serial.begin(115200);                     // UART0 (connected to USB by default)
  Serial2.begin(9600, SERIAL_8N1, 16, 17);  // UART2: RX=GPIO16, TX=GPIO17
}
 
/**
 * @brief Main loop, runs repeatedly.
 */
void loop() {
  unsigned long currentMillis = millis();
 
  // Send messages every second
  if (currentMillis - lastUART >= intervalUART) {
    lastUART = currentMillis;
    Serial.println("Message from UART0");
    Serial2.println("Message from UART2");
  }
}
