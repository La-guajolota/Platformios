/****************************************************************************************
 * @file main.cpp
 * @brief This program implements a PID control system to regulate the temperature of a
 *        system using a Solid State Relay (SSR). The temperature is measured with a DHT
 *        sensor (11 or 22), and the PID controller adjusts the activation time of the
 *        SSR based on the difference between the measured temperature and a predefined setpoint.
 *
 * @author Adrian Silva Palafox
 * @company Fourie Embeds
 * @date November 2024
 *
 * @license Free code under MIT. It can be modified and distributed under the terms
 *          established by the license.
 ***************************************************************************************/
#include <Arduino.h>
#include "TimerOne.h" // Library for using Arduino's Timer 1
#include "DHT.h"      // Library for the DHT sensor (temperature and humidity)
#include "PID.hpp"    // Custom library for the PID controller

// Macros
#define MAX_HALF_CYCLES 120 // Maximum number of half-cycles (for relay control)

// Pin Configuration (GPIO)
#define ZC_PIN 3   // Pin to detect zero crossing (used to synchronize power control)
#define FIRE_PIN 7 // Pin to control the SSR (Solid State Relay) trigger
#define SENSOR_PIN 6   // Pin to connect the DHT sensor (temperature and humidity)

/**************************** PID Parameters ********************************/
// PID controller parameters
float KP = 15.0;  // Proportional gain
float KI = 0.25;  // Integral gain
float KD = 0.005; // Derivative gain

// Derivative low-pass filter constant
const float TAU = 0.25; // Time constant for the derivative filter

// PID sampling time (in seconds)
const float T_SAMPLE = 1;

// Limits for the PID controller output
const float LIM_MIN = 0.0;            // Minimum output limit (ON cycles)
const float LIM_MAX = MAX_HALF_CYCLES; // Maximum output limit (ON cycles)

// Limits for the integrator (to prevent wind-up)
const float LIM_MIN_INT = -100.00; // Lower limit for the integrator
const float LIM_MAX_INT = 100.0;   // Upper limit for the integrator

// Setpoint (target temperature)
float SETPOINT = 37.0; // Desired temperature (in degrees Celsius)

// PID controller instance
PIDController pid(KP, KI, KD, TAU, LIM_MIN, LIM_MAX, LIM_MIN_INT, LIM_MAX_INT, T_SAMPLE);

/**************************** Control Variables ********************************/
// Variables related to interrupts and control
volatile bool flagSense = false; // Flag to indicate that a measurement should be taken
volatile bool flagZC = false;    // Flag to indicate that a zero crossing has occurred

float temp = 0;         // Variable to store the measured temperature
uint8_t ciclosOn = 0;   // Number of ON cycles (controlled by PID)
uint8_t contCiclos = 0; // Current cycle counter (based on zero crossing)

// DHT sensor instance
DHT sensor; // Initialize the DHT sensor (can be changed to DHT22 if necessary)

/**************************** Function Prototypes ********************************/
void ZC_ISR();                               // Interrupt service routine for zero crossing
void SENSE_ISR();                            // Interrupt service routine for sensor reading
void parseCommand(String command);           // Function to interpret serial commands
float parseValue(String command, char type); // Function to extract a value from a command
void printGains();                           // Function to print the gain and setpoint values

void setup()
{
  // Initialize serial communication at 9600 baud
  Serial.begin(9600);

  // Configure input and output pins
  pinMode(ZC_PIN, INPUT_PULLUP); // Configure the zero-crossing pin as an input with a pull-up resistor
  pinMode(FIRE_PIN, OUTPUT);     // Configure the SSR pin as an output

  // Configure zero-crossing interrupt
  attachInterrupt(digitalPinToInterrupt(ZC_PIN), ZC_ISR, FALLING); // Detect the falling edge of the zero-crossing signal

  // Configure Timer1 to generate interrupts every T_SAMPLE seconds
  Timer1.initialize(T_SAMPLE * 1e6); // Set up Timer1 to trigger every T_SAMPLE seconds
  Timer1.attachInterrupt(SENSE_ISR); // Associate the sensor reading interrupt with the timer

  // Configure the DHT sensor
  sensor.setup(SENSOR_PIN); // Set up the DHT sensor on the defined pin

  // Initialize the output (turn off the SSR at the beginning)
  digitalWrite(FIRE_PIN, LOW);

  // Startup message
  // Serial.println("PID Controller Initialized.");
  // printGains(); // Print the initial PID parameters
}

void loop()
{
  // If there is data available on the serial port, process it
  if (Serial.available() > 0)
  {
    String incomingCommand = Serial.readStringUntil('\n'); // Read the entire line until a newline character
    parseCommand(incomingCommand);                         // Interpret the received command
    // printGains();                                          // Print the current setpoint and gain values
  }

  // If the sensing flag is activated, read the temperature and update the PID
  if (flagSense)
  {
    flagSense = false; // Reset the flag

    temp = sensor.getTemperature(); // Get the temperature from the sensor

    // Check if the temperature reading was successful
    if (isnan(temp))
    {
      Serial.println("Error: Could not read temperature.");
      temp = 0;     // Reset temperature in case of an error
      ciclosOn = 0; // Deactivate cycles
    }

    // Calculate the number of ON cycles from the PID output
    ciclosOn = static_cast<uint8_t>(pid.update(SETPOINT, temp)); // Update the PID output

    // Print the temperature and setpoint to the serial port
    Serial.print(temp);
    Serial.print(",");
    Serial.println(SETPOINT);
  }

  // If a zero crossing was detected, handle the SSR on/off state
  if (flagZC)
  {
    flagZC = false; // Reset the flag

    // Increment the cycle counter, and reset it if it reaches the maximum
    if (++contCiclos >= MAX_HALF_CYCLES)
    {
      contCiclos = 0;
    }

    // Control the state of the SSR (relay) depending on the calculated ON cycles
    digitalWrite(FIRE_PIN, (contCiclos < ciclosOn) ? HIGH : LOW); // Turn the SSR on/off
  }
}

// Interrupt for zero crossing
void ZC_ISR() { flagZC = true; }

// Interrupt for sensor reading (timer)
void SENSE_ISR() { flagSense = true; }

// Function to parse a numeric value from a command
float parseValue(String command, char type)
{
  // Check if the command contains a numeric value
  if (command.length() > 1 && isDigit(command[1]))
  {
    return command.substring(1).toFloat(); // Extract the numeric value
  }
  else
  {
    Serial.print("Error: invalid value for ");
    Serial.println(type); // Error message if no valid value is found
    return NAN;           // Return "Not-a-Number" if the value is not valid
  }
}

// Function to interpret serial commands
void parseCommand(String command)
{
  if (command.startsWith("P")) // Command to adjust KP
  {
    float value = parseValue(command, 'P');
    if (!isnan(value))
      KP = value; // Update the value of KP
  }
  else if (command.startsWith("I")) // Command to adjust KI
  {
    float value = parseValue(command, 'I');
    if (!isnan(value))
      KI = value; // Update the value of KI
  }
  else if (command.startsWith("D")) // Command to adjust KD
  {
    float value = parseValue(command, 'D');
    if (!isnan(value))
      KD = value; // Update the value of KD
  }
  else if (command.startsWith("S")) // Command to adjust the setpoint
  {
    float value = parseValue(command, 'S');
    if (!isnan(value))
      SETPOINT = value; // Update the setpoint
  }
  else
  {
    Serial.println("Invalid command."); // Error message if the command is not recognized
  }

  // Update the PID controller gains with the new values
  pid.updateGains(KP, KI, KD);
}

// Function to print the current values of the gains and the setpoint
void printGains()
{
  Serial.print("Setpoint: ");
  Serial.println(SETPOINT);
  Serial.print("KP: ");
  Serial.println(KP);
  Serial.print("KI: ");
  Serial.println(KI);
  Serial.print("KD: ");
  Serial.println(KD);
}