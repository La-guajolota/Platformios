/**
 * @file     main.c
 * @author   Adrián Silva Palafox
 * @date     2024-11-30
 * @version  0.2
 * @brief    Controlador de 6 IGBTs para un inversor trifásico basado en Arduino.
 * @details  Este programa implementa un registro circular para generar la secuencia de disparo
 *           de 6 IGBTs conectados al puerto B de un microcontrolador ATmega328P (Arduino Uno).
 *           Está diseñado para convertir voltaje DC en una señal AC trifásica utilizando
 *           conmutación en 3 pares de IGBTs con desfase entre ellos.
 *
 *           La conmutación está programada con un período fijo definido en microsegundos,
 *           y cada par de IGBTs está configurado en modo complementario.
 *
 * @note     Este código se puede extender para integrar modulación PWM con un desfase
 *           de 120° entre las fases. Para evitar cortocircuitos en los IGBTs, se recomienda
 *           incluir circuitos dead-time en el hardware.
 *
 * @dependencies
 *           - Plataforma: Arduino Uno (ATmega328P)
 *           - Software: Arduino IDE o cualquier editor compatible con AVR-GCC
 *
 * @usage    Este programa está diseñado para operar en sistemas de potencia con inversores
 *           trifásicos. Asegúrate de conectar los IGBTs al puerto B según la configuración del
 *           registro y utilizar circuitos de aislamiento adecuados (optoacopladores o drivers).
 *
 * @warning  Asegúrate de verificar la sincronización y la frecuencia de conmutación para evitar
 *           daños en los componentes electrónicos y asegurar una salida de potencia adecuada.
 */
#include <Arduino.h>

// Configuración
// #define DEBUG_MODE          // Comentar para desactivar el modo de depuración
#define IGBT_PERIOD_US 1388 // Tiempo entre activaciones en microsegundos
#define MASK 0b00111111     // Pines PB0-PB5 como salidas

// Variables globales
uint8_t reg = 0b00000111; // Registro inicial con 3 bits en HIGH

// Funciones
void nextCommutation();

void setup()
{
  /*
  PB0 -> PIN8  igbt_1  |  PB1 -> PIN9  igbtB_3 | PB2 -> PIN10 igbtC_5
  PB3 -> PIN11 igbt_4  |  PB4 -> PIN12 igbtB_6 | PB5 -> PIN13 igbtC_2
  */
  DDRB |= MASK; // Configurar PB0-PB5 como salidas
  PORTB = 0x00; // Inicializar en LOW

#ifdef DEBUG_MODE
  Serial.begin(9600);
  Serial.println("Controlador de inversor iniciado.");
  Serial.println(F_CPU); // Muestra la frecuencia de reloj en Hz
#endif

  delay(1000); // Retraso inicial
}

void loop()
{
  nextCommutation();

#ifdef DEBUG_MODE
  Serial.print("Registro actual: ");
  for (int8_t i = 5; i >= 0; i--)
  {
    Serial.print((reg >> i) & 1);
  }
  Serial.println();
  delay(1000); // Tiempo adicional para observar el estado
#endif
}

void nextCommutation()
{
  // Extraer el 5º bit, desplazarlo y combinarlo con el resto
  reg = ((reg << 1) | ((reg >> 5) & 0b1)) & MASK;

  PORTB = reg; // Actualizar las salidas
  delayMicroseconds(IGBT_PERIOD_US);
}
