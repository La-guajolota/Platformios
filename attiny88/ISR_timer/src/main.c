/*
  @author Adrián Silva Palafox
  @date   Dec 12 2024
  @brief  Código para parpadear un LED usando interrupciones con un temporizador en AVR.
*/

#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000                     //*Frecuencia del microcontrolador (16 MHz)
#define COUNTER_SET 65535 - (F_CPU / 1024) //*Ticks suficientes para un 1 seg

//*Interrupción por desbordamiento del Timer1
ISR(TIMER1_OVF_vect)
{
  PORTD ^= _BV(PORTD0); //*Alterna el estado del LED en PD0
  TCNT1 = COUNTER_SET;  //*Reinicia el temporizador
}

int main(void)
{
  //*Configura PD0 como salida para el LED
  DDRD |= _BV(DD0);

  //*Configuración del Timer1: prescaler 1024
  TCNT1 = COUNTER_SET;
  TCCR1B = (1 << CS10) | (1 << CS12); //*Inicia el temporizador con prescaler 1024
  TCCR1A = 0;                         //*Sin configuración especial para TCCR1A
  TIMSK1 = (1 << TOIE1);              //*Habilita la interrupción por desbordamiento
  sei();                              //*Habilita interrupciones globales

  while (1)
  {
    //*El parpadeo del LED se maneja por interrupciones, el MCU puede hacer otras tareas aquí.
  }
}
