/**
 * @file main.cpp
 * @brief RF transmitter using a joystick and an NRF24L01 module.
 * 
 * This program reads the values from a joystick and sends them via an NRF24L01 module.
 * 
 * @author Gilberto Prado | Adrián Silva Palafox
 * @date February 19, 2025
 */

#include <Arduino.h>
#include <SPI.h>
#include "RF24.h"
#include "nRF24L01.h"

// Uncomment the board you are using
//#define ARDUINO_UNO
#define ARDUINO_NANO

// Uncomment the desired operation mode
#define DEBUG_MODE
//#define COMPETITION_MODE

// Pin Definitions
#ifdef ARDUINO_UNO
  #define CE_PIN 8
  #define CSN_PIN 9
  #define VRX A0  
  #define VRY A1
#elif defined(ARDUINO_NANO)
  /** Chip Enable pin for the RF module */
  #define CE_PIN 9
  /** Chip Select pin for the RF module */
  #define CSN_PIN 10
  /** Joystick X-axis pin */
  #define VRX A0  
  /** Joystick Y-axis pin */
  #define VRY A1
#endif

// RF Module Constructor
/** RF24 object for RF communication */
RF24 radio(CE_PIN, CSN_PIN);

// Data structure for sending data
/**
 * @struct Data_to_be_sent
 * @brief Structure to store the data to be sent.
 * 
 * This structure contains two data channels representing the joystick readings.
 */
struct Data_to_be_sent {
  byte ch1; /**< Channel 1: Joystick X-axis reading */
  byte ch2; /**< Channel 2: Joystick Y-axis reading */
} sent_data;

// Global Variables
/** RF communication pipe address */
const uint64_t my_radio_pipe = 0xE8E8F0F0E1LL;

/**
 * @brief Setup function, runs once at the beginning.
 */
void setup() {
#ifdef DEBUG_MODE
  Serial.begin(9600);
#endif

  // Initialize the RF module
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(my_radio_pipe);
}

/**
 * @brief Main loop, runs repeatedly.
 */
void loop() {
  // Read joystick values and map them to a 0-255 range
  sent_data.ch1 = map(analogRead(VRX), 0, 1023, 0, 255);
  sent_data.ch2 = map(analogRead(VRY), 0, 1023, 0, 255);

#ifdef DEBUG_MODE
  // Print joystick values to the serial monitor in debug mode
  Serial.print("X: ");
  Serial.print(sent_data.ch1);
  Serial.print(" Y: ");
  Serial.println(sent_data.ch2);
  delay(250);
#endif

  // Send the data
  radio.write(&sent_data, sizeof(Data_to_be_sent));
}
