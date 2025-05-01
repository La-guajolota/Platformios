/**
 * @file     main.c
 * @author   Adrián Silva Palafox
 * @date     2024-12-01
 * @version  0.3
 * @brief    Controlador de disparo para SCRs en un rectificador controlado de 4 SCRs basado en Arduino.
 * @details  Este programa implementa un sistema de disparo para un rectificador controlado de 4 SCRs. El disparo de los
 *           SCRs se realiza mediante la detección de cambios de nivel en el pin de sincronización. Se utiliza un retardo
 *           ON-DELAY configurable que se ajusta dinámicamente.
 *
 *           El sistema se basa en interrupciones para controlar el temporizador de forma precisa, optimizando el tiempo de
 *           respuesta y evitando bloqueos. El retardo para cada disparo está basado en el valor de DELAY_DISPARO_US, el cual
 *           se puede ajustar de forma continua.
 *
 * @note     Este código está diseñado para ser fácilmente adaptable a otros sistemas de potencia. Se recomienda utilizar
 *           circuitos de aislamiento (optoacopladores o drivers) para proteger los SCRs y el microcontrolador.
 *
 * @dependencies
 *           - Plataforma: Arduino Uno (ATmega328P)
 *           - Software: Arduino IDE o cualquier editor compatible con AVR-GCC
 *
 * @usage    Este programa se puede utilizar en sistemas de rectificación controlada para obtener un disparo preciso de los
 *           SCRs. Asegúrate de conectar los SCRs adecuadamente y de configurar correctamente los valores de retardo para evitar
 *           posibles daños en los componentes.
 *
 * @warning  Asegúrate de verificar los valores de DELAY_DISPARO_US para evitar problemas en la sincronización de los SCRs
 *           y asegurar la correcta operación del sistema.
 */
#include <Arduino.h>

// Definiciones
#define PULSO_DISPARO_US 10 // Duración del pulso de disparo en microsegundos
#define MAX_DELAY 16000     // (16000 ticks * 0.5 µs) = 8ms

// Variables globales
volatile bool dispararSCR1 = false;      // Bandera para disparar Par SCR1
volatile bool dispararSCR2 = false;      // Bandera para disparar Par SCR2
volatile uint16_t DELAY_DISPARO_US = 50; // ON-DELAY -> DELAY_DISPARO_US * 10us
int i;                                   // Auxiliar para los ciclos for

void setup()
{
  // Configuración GPIO
  DDRB = 0b110;      // PB1 y PB2 como salidas (SCRs), PB0 como entrada (sincronización)
  PORTB &= ~(0b110); // Salidas inicien en LOW

  // Configurar interrupción por cambio de pin
  PCICR |= (1 << PCIE0);   // Habilita interrupciones para el Banco 0
  PCMSK0 |= (1 << PCINT0); // Habilita interrupción para PB0
  sei();                   // Habilitar interrupciones globales

  // Configurar Timer1 en modo CTC (Clear Timer on Compare Match)
  TCCR1A = 0;              // Modo normal sin PWM
  TCCR1B = (1 << WGM12);   // Modo CTC
  TCCR1B |= (1 << CS11);   // Prescaler de 8 (1 tick = 0.5 µs)
  OCR1A = 16000;           // Comparador para 8 ms (16000 ticks * 0.5 µs) RMS cercano a 0
  TIMSK1 |= (1 << OCIE1A); // Habilitar interrupción del comparador A
}

void loop()
{
  // Barrido de angulos de disparo
  /* De o a 179 grados*/
  for (i = 0; i <= MAX_DELAY; i += 100)
  {
    DELAY_DISPARO_US = i;
    delay(25);
  }

  /* de 179 a 0 grados*/
  for (i = MAX_DELAY; i >= 1; i -= 100)
  {
    DELAY_DISPARO_US = i;
    delay(25);
  }
}

// Interrupción por cambio de pin (detección de sincronización)
ISR(PCINT0_vect)
{
  TCNT1 = 0;             // Reinicia el contador del Timer1
  if (PINB & (1 << PB0)) // Detecta flanco ascendente en PB0
  {
    dispararSCR1 = true; // Establece que se debe disparar PAR SCR1
  }
  else // Flanco descendente en PB0
  {
    dispararSCR2 = true; // Establece que se debe disparar PAR SCR2
  }
  OCR1A = DELAY_DISPARO_US; // Configura el retardo para el disparo
}

// Interrupción del Timer1 (para disparar SCRs)
ISR(TIMER1_COMPA_vect)
{
  if (dispararSCR1)
  {
    PORTB |= (1 << PB1);                 // Activa PAR SCR1
    delayMicroseconds(PULSO_DISPARO_US); // Pulso de disparo
    PORTB &= ~(1 << PB1);                // Apaga SCR1
    dispararSCR1 = false;                // Resetea la bandera
  }
  else if (dispararSCR2)
  {
    PORTB |= (1 << PB2);                 // Activa PAR SCR2
    delayMicroseconds(PULSO_DISPARO_US); // Pulso de disparo
    PORTB &= ~(1 << PB2);                // Apaga SCR2
    dispararSCR2 = false;                // Resetea la bandera
  }
}
