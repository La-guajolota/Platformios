/**
 * @file     main.cpp
 * @author   Adrián Silva Palafox
 * @date     2024-11-30
 * @version  0.3
 * @brief    Six-IGBT driver for a three-phase inverter using Arduino.
 * @details  This program implements a circular shift register to generate the firing sequence
 *           for 6 IGBTs connected to port B of an ATmega328P microcontroller (Arduino Uno).
 *           It is designed to convert DC voltage into a three-phase AC signal by switching
 *           3 pairs of IGBTs with a phase shift between them.
 *
 *           The switching is programmed with a fixed period defined in microseconds,
 *           and each pair of IGBTs is configured in a complementary mode.
 *
 * @note     This code can be extended to integrate PWM modulation with a 120° phase shift
 *           between the phases. To prevent short circuits in the IGBTs, it is recommended
 *           to include dead-time circuits in the hardware.
 *
 * @dependencies
 *           - Platform: Arduino Uno (ATmega328P)
 *           - Software: Arduino IDE or any AVR-GCC compatible editor
 *
 * @usage    This program is designed for power systems with three-phase inverters.
 *           Ensure that the IGBTs are connected to port B according to the register
 *           configuration and use appropriate isolation circuits (optocouplers or drivers).
 *
 * @warning  Make sure to verify the synchronization and switching frequency to avoid
 *           damage to electronic components and ensure proper power output.
 */
#include <Arduino.h>

// Configuration
// #define DEBUG_MODE          // Uncomment to enable debug mode
#define IGBT_PERIOD_US 1388 // Time between activations in microseconds
#define MASK 0b00111111     // Use pins PB0-PB5 as outputs

// Global variables
uint8_t reg = 0b00000111; // Initial register with 3 bits set to HIGH

// Function prototypes
void nextCommutation();

/**
 * @brief Initializes the microcontroller.
 * @details Sets up the pins connected to the IGBTs as outputs and initializes them to LOW.
 *          If DEBUG_MODE is enabled, it starts serial communication.
 */
void setup()
{
  /*
   * Pin configuration:
   * PB0 -> Arduino PIN 8  -> IGBT 1
   * PB1 -> Arduino PIN 9  -> IGBT 3 (Phase B)
   * PB2 -> Arduino PIN 10 -> IGBT 5 (Phase C)
   * PB3 -> Arduino PIN 11 -> IGBT 4
   * PB4 -> Arduino PIN 12 -> IGBT 6 (Phase B)
   * PB5 -> Arduino PIN 13 -> IGBT 2 (Phase C)
   */
  DDRB |= MASK; // Configure pins PB0-PB5 as outputs
  PORTB = 0x00; // Initialize all outputs to LOW

#ifdef DEBUG_MODE
  Serial.begin(9600);
  Serial.println("Inverter controller initialized.");
  Serial.print("Clock Frequency (F_CPU): ");
  Serial.println(F_CPU); // Display the clock frequency in Hz
#endif

  delay(1000); // Initial delay for stability
}

/**
 * @brief Main loop of the program.
 * @details Continuously calls the nextCommutation() function to update the IGBT states.
 *          If DEBUG_MODE is enabled, it prints the current state of the register to the serial monitor.
 */
void loop()
{
  nextCommutation();

#ifdef DEBUG_MODE
  Serial.print("Current register: ");
  for (int8_t i = 5; i >= 0; i--)
  {
    Serial.print((reg >> i) & 1);
  }
  Serial.println();
  delay(1000); // Additional delay to observe the state in debug mode
#endif
}

/**
 * @brief Performs the next commutation of the IGBTs.
 * @details This function implements a circular left shift on the `reg` variable.
 *          The 6th bit is moved to the 0th position, and all other bits are shifted left.
 *          The result is masked to 6 bits and written to PORTB to control the IGBTs.
 *          A delay is introduced to control the switching frequency.
 */
void nextCommutation()
{
  // Perform a circular left shift
  // Extract the 5th bit, shift it to the 0th position, and combine it with the rest of the shifted register
  reg = ((reg << 1) | ((reg >> 5) & 0b1)) & MASK;

  PORTB = reg; // Update the outputs
  delayMicroseconds(IGBT_PERIOD_US); // Wait for the defined period
}