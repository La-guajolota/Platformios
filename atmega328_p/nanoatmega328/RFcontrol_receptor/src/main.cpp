/**
 * @file main.cpp
 * @author Adrián Silva Palafox
 * @brief RF remote control receiver for a 2-wheel robot.
 * @details This code runs on an Arduino Nano and receives commands from an RF transmitter
 *          to control a 2-wheeled robot using a TB6612 motor driver.
 */

#include <Arduino.h>
#include <SPI.h>
#include "RF24.h"
#include "nRF24L01.h"

// Pin Definitions
/* SPI for NRF24L01 */
#define CE_PIN 9
#define CSN_PIN 10
/* TB6612 Motor Driver Pins */
#define AIN1 5 
#define AIN2 4
#define PWMA 7
#define BIN1 2
#define BIN2 3
#define PWMB 8
#define STBY 6

// RF Module Constructor
RF24 radio(CE_PIN, CSN_PIN);

// Data structure for receiving data from the transmitter
struct Received_data {
  byte ch1; // Channel 1 (Forward/Backward)
  byte ch2; // Channel 2 (Left/Right)
} received_data;

// Global Variables
const uint64_t my_radio_pipe = 0xE8E8F0F0E1LL; // Radio pipe address for communication
unsigned long lastRecvTime = 0; // Time when the last data was received
const int max_pwm = 255;
const int min_pwm = -255;
const int max_ctl_rf = 2000;
const int min_ctl_rf = 950;
int forwardBackward; // Stores forward/backward speed and direction
int leftRight;       // Stores left/right speed and direction
int speed1;          // Speed for motor 1
int speed2;          // Speed for motor 2

// Function Prototypes
void receive_the_data();
void setMotor(int pwmPin, int in1, int in2, int speed);
void stopIfFault();
void reset_the_Data();

/**
 * @brief Setup function, runs once at the beginning.
 */
void setup() {
  // Initialize Serial Communication
  Serial.begin(9600);
  
  // GPIO Configuration
  /* Set motor driver pins as outputs */
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(STBY, OUTPUT);

  digitalWrite(STBY, HIGH); // Enable the TB6612 driver

  // Initialize RF module
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(1, my_radio_pipe); // Corrected from openWritingPipe

  radio.startListening();
}

/**
 * @brief Main loop, runs repeatedly.
 */
void loop() {
  receive_the_data();

  // Check if the signal is lost
  unsigned long now = millis();
  if ( now - lastRecvTime > 1000 ) {
    reset_the_Data();
  } 

  // Map joystick values to motor speed range
  forwardBackward = map(received_data.ch1, min_ctl_rf, max_ctl_rf, min_pwm, max_pwm);
  leftRight = map(received_data.ch2, min_ctl_rf, max_ctl_rf, min_pwm, max_pwm);

  // Combine forward/backward and left/right values to get individual motor speeds
  speed1 = forwardBackward + leftRight;
  speed2 = forwardBackward - leftRight;

  // Constrain motor speeds to valid PWM range
  speed1 = constrain(speed1, min_pwm, max_pwm);
  speed2 = constrain(speed2, min_pwm, max_pwm);

  // Set motor speeds
  setMotor(PWMA, AIN1, AIN2, speed1);
  setMotor(PWMB, BIN1, BIN2, speed2);
} 

/**
 * @brief Receives data from the RF module.
 */
void receive_the_data()
{
  while ( radio.available() ) {
    radio.read(&received_data, sizeof(Received_data));
    lastRecvTime = millis(); // Update the time of the last received data
  }
}

/**
 * @brief Placeholder for fault checking.
 * @note The TB6612 driver does not have a fault pin, so this function is not implemented.
 */
void stopIfFault()
{
  Serial.println("Checking for faults (not available in TB6612)");
}

/**
 * @brief Sets the speed and direction of a motor.
 * @param pwmPin The PWM pin for the motor.
 * @param in1 The first direction pin.
 * @param in2 The second direction pin.
 * @param speed The desired speed (-255 to 255).
 */
void setMotor(int pwmPin, int in1, int in2, int speed)
{
  if (speed > 0) // Forward motion
  {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  }
  else if (speed < 0) // Backward motion
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  }
  else // Stop the motor
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }
  analogWrite(pwmPin, abs(speed)); // Set the motor speed
}

/**
 * @brief Resets the received data to a default state.
 */
void reset_the_Data() 
{
  received_data.ch1 = 1500; // Center value for joystick
  received_data.ch2 = 1500; // Center value for joystick
}
