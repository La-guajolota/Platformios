/**
 * @file main.c
 * @author Adrian Silva Palafox
 * @date 2024
 * @brief Main C file to call an external assembly function.
 * @details This program configures PD0 as an output and then enters an infinite loop
 *          that calls an assembly function `_start` to blink a LED.
 */

#define F_CPU 16000000UL // 16MHz clock frequency
#include <avr/io.h>
#include <util/delay.h>

// Declare the external assembly function
extern void _start(void);

/**
 * @brief Main function.
 * @details Configures PD0 as an output and calls the assembly function in a loop.
 */
int main(void)
{
  // Configure PD0 as an output
  DDRD |= (1 << PD0);

  // Infinite loop to call the assembly function
  while (1)
  {
    _start();
  }
}