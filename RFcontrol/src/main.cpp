/**
 * @file main.cpp
 * @brief Control de RF utilizando un joystick y el módulo RF24.
 * 
 * Este programa lee los valores de un joystick y los envía a través de un módulo RF24.
 * 
 * @author Gilberto Prado | Adrián Silva Palafox
 * @date 19 de Febrero de 2025
 */

#include <Arduino.h>
#include <SPI.h>
#include "RF24.h"
#include "nRF24L01.h"

// Descomentar la tarjeta que se usará
//#define ARDUINO_UNO
#define ARDUINO_NANO

//Descomentar el modo de operación
#define DEBUG_MODE
//#define COMPETENCIA_MODE

// DEFINICIONES DE LOS PINES
#ifdef ARDUINO_UNO
  #define CE_PIN 8
  #define CSN_PIN 9
  #define VRX A0  
  #define VRY A1
#elif defined(ARDUINO_NANO)
  /** Pin de habilitación del módulo RF */
  #define CE_PIN 9
  /** Pin de selección de chip del módulo RF */
  #define CSN_PIN 10
  /** Pin del eje X del joystick */
  #define VRX A0  
  /** Pin del eje Y del joystick */
  #define VRY A1
#endif

// CONSTRUCTOR PARA EL MODULO DE RF
/** Objeto RF24 para la comunicación RF */
RF24 radio(CE_PIN, CSN_PIN);

// STRUCT
/**
 * @struct Data_to_be_sent
 * @brief Estructura para almacenar los datos a enviar.
 * 
 * Esta estructura contiene dos canales de datos que representan las lecturas del joystick.
 */
struct Data_to_be_sent {
  byte ch1; /**< Canal 1: Lectura del eje X del joystick */
  byte ch2; /**< Canal 2: Lectura del eje Y del joystick */
} sent_data;

// VARIABLES GLOBALES
/** Dirección del pipe de comunicación RF */
const uint64_t my_radio_pipe = 0xE8E8F0F0E1LL;

void setup() {
#ifdef DEBUG_MODE
  Serial.begin(9600);
#endif

  // Inicializamos el módulo RF
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(my_radio_pipe);
}

void loop() {
  sent_data.ch1 = map(analogRead(VRX), 0, 1024, 0, 255);
  sent_data.ch2 = map(analogRead(VRY), 0, 1024, 0, 255);

#ifdef DEBUG_MODE
  Serial.print("X: ");
  Serial.print(sent_data.ch1);
  Serial.print(" Y: ");
  Serial.println(sent_data.ch2);
  delay(250);
#endif

  radio.write(&sent_data, sizeof(Data_to_be_sent));
}