#define F_CPU 16000000UL // 1MHz default clock
#include <avr/io.h>
#include <util/delay.h>

extern void _start(void); // Declarar la función de ensamblador externa

int main(void)
{
  DDRD |= (1 << PD0); // Configurar PD0 como salida

  while (1)
  {
    _start(); // Llamar a la función de ensamblador
  }
}
