#include <Arduino.h>

// TA8080 GPIO
#define DI1 1
#define DI2 2
// Motor control combinations
#define Brake 0
#define Forward 1
#define Backward 2
#define Stop 3

// Function to set motor driver state
void motorDriverSetup(uint8_t combination);

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize GPIO pins
  pinMode(DI1, OUTPUT);
  pinMode(DI2, OUTPUT);
}

void loop() {
  // Wait for user input to set motor state
  Serial.println("Enter motor command (0: Brake, 1: Forward, 2: Backward, 3: Stop):");
  while (!Serial.available());
  uint8_t command = Serial.read();

  motorDriverSetup(command);
}

void motorDriverSetup(uint8_t combination) {
  switch (combination) {
    case 0:
      Serial.println("Brake");
      digitalWrite(DI1, HIGH);
      digitalWrite(DI2, HIGH);
      break;
    case 1:
      Serial.println("Forward");
      digitalWrite(DI1, LOW);
      digitalWrite(DI2, HIGH);
      break;
    case 2:
      Serial.println("Backward");
      digitalWrite(DI1, HIGH);
      digitalWrite(DI2, LOW);
      break;
    case 3:
      Serial.println("Stop");
      digitalWrite(DI1, LOW);
      digitalWrite(DI2, LOW);
      break;
    default:
      // Invalid combination, do nothing or handle error
      break;
  }
}