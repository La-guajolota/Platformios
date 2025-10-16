/**
 * @file     main.cpp
 * @author   Adrián Silva Palafox
 * @date     2024-12-01
 * @version  0.4
 * @brief    Trigger controller for SCRs in a 4-SCR controlled rectifier based on Arduino.
 * @details  This program implements a firing system for a 4-SCR controlled rectifier. The SCRs are triggered
 *           by detecting level changes on the synchronization pin. It uses a configurable ON-DELAY that can be
 *           adjusted dynamically.
 *
 *           The system is based on interrupts to control the timer precisely, optimizing response time and
 *           avoiding blocking calls. The delay for each trigger is based on the value of DELAY_DISPARO_US, which
 *           can be adjusted continuously.
 *
 * @note     This code is designed to be easily adaptable to other power systems. It is recommended to use
 *           isolation circuits (optocouplers or drivers) to protect the SCRs and the microcontroller.
 *
 * @dependencies
 *           - Platform: Arduino Uno (ATmega328P)
 *           - Software: Arduino IDE or any AVR-GCC compatible editor
 *
 * @usage    This program can be used in controlled rectification systems to achieve precise triggering of SCRs.
 *           Make sure to connect the SCRs properly and configure the delay values correctly to avoid
 *           potential damage to the components.
 *
 * @warning  Ensure that the DELAY_DISPARO_US values are verified to avoid synchronization problems with the SCRs
 *           and to ensure the correct operation of the system.
 */
#include <Arduino.h>

// Definitions
#define PULSE_WIDTH_US 10 // Duration of the trigger pulse in microseconds
#define MAX_DELAY 16000     // (16000 ticks * 0.5 µs) = 8ms

// Global variables
volatile bool fireSCR1 = false;      // Flag to fire SCR pair 1
volatile bool fireSCR2 = false;      // Flag to fire SCR pair 2
volatile uint16_t trigger_delay_us = 50; // ON-DELAY -> trigger_delay_us * 10us
int i;                                   // Loop counter

/**
 * @brief Initializes the microcontroller and peripherals.
 */
void setup()
{
  // GPIO Configuration
  DDRB = 0b00000110;      // PB1 and PB2 as outputs (for SCRs), PB0 as input (for synchronization)
  PORTB &= ~(0b00000110); // Initialize outputs to LOW

  // Configure pin change interrupt for synchronization
  PCICR |= (1 << PCIE0);   // Enable pin change interrupts for bank 0
  PCMSK0 |= (1 << PCINT0); // Enable interrupt for PB0
  sei();                   // Enable global interrupts

  // Configure Timer1 in CTC (Clear Timer on Compare Match) mode
  TCCR1A = 0;              // Normal mode, no PWM
  TCCR1B = (1 << WGM12);   // CTC mode
  TCCR1B |= (1 << CS11);   // Prescaler of 8 (1 tick = 0.5 µs)
  OCR1A = 16000;           // Compare value for 8 ms (16000 ticks * 0.5 µs)
  TIMSK1 |= (1 << OCIE1A); // Enable compare match A interrupt
}

/**
 * @brief Main loop to vary the firing angle.
 */
void loop()
{
  // Sweep the firing angle from 0 to 179 degrees
  for (i = 0; i <= MAX_DELAY; i += 100)
  {
    trigger_delay_us = i;
    delay(25);
  }

  // Sweep the firing angle from 179 to 0 degrees
  for (i = MAX_DELAY; i >= 1; i -= 100)
  {
    trigger_delay_us = i;
    delay(25);
  }
}

/**
 * @brief Pin change interrupt service routine for synchronization.
 */
ISR(PCINT0_vect)
{
  TCNT1 = 0;             // Reset Timer1 counter
  if (PINB & (1 << PB0)) // Detect rising edge on PB0
  {
    fireSCR1 = true; // Set flag to fire SCR pair 1
  }
  else // Falling edge on PB0
  {
    fireSCR2 = true; // Set flag to fire SCR pair 2
  }
  OCR1A = trigger_delay_us; // Set the delay for the trigger
}

/**
 * @brief Timer1 compare match interrupt service routine to fire the SCRs.
 */
ISR(TIMER1_COMPA_vect)
{
  if (fireSCR1)
  {
    PORTB |= (1 << PB1);                 // Activate SCR pair 1
    delayMicroseconds(PULSE_WIDTH_US); // Trigger pulse
    PORTB &= ~(1 << PB1);                // Deactivate SCR pair 1
    fireSCR1 = false;                // Reset the flag
  }
  else if (fireSCR2)
  {
    PORTB |= (1 << PB2);                 // Activate SCR pair 2
    delayMicroseconds(PULSE_WIDTH_US); // Trigger pulse
    PORTB &= ~(1 << PB2);                // Deactivate SCR pair 2
    fireSCR2 = false;                // Reset the flag
  }
}