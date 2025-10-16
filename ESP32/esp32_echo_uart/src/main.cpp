/**
 * @file main.cpp
 * @author Adrian Silva Palafox
 * @date 2024
 * @brief A simple UART echo program for the ESP32.
 * @details This program reads data from one serial port and writes it to another, effectively
 *          creating a bridge between a PC and another microcontroller.
 */

#include <Arduino.h>

/**
 * @brief Setup function, runs once at the beginning.
 */
void setup() {
  // Initialize Serial for communication with the PC
  Serial.begin(115200);
  
  // Initialize Serial1 for communication with the external MCU
  // RX = GPIO16, TX = GPIO17
  Serial1.begin(115200, SERIAL_8N1, 16, 17);
}

/**
 * @brief Main loop, runs repeatedly.
 */
void loop() {
  // Read data from the external MCU and send it to the PC
  if (Serial1.available()) {
    int data = Serial1.read();
    Serial.write(data);
  }

  // Read data from the PC and send it to the external MCU
  if (Serial.available()) {
    int data = Serial.read();
    Serial1.write(data);
  }
}