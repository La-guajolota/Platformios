/****************************************************************************************
 * Archivo: ControladorPID.ino
 * Descripción: Este programa implementa un sistema de control PID para regular la
 *              temperatura de un sistema mediante un relé controlado por un SSR (Solid
 *              State Relay). La temperatura se mide con un sensor DHT (11 o 22), y el
 *              controlador PID ajusta el tiempo de activación del SSR en función de la
 *              diferencia entre la temperatura medida y un setpoint predefinido.
 *
 * Autor: Adrian Silva Palafox
 * Empresa: Fourie Embeds
 * Fecha: Noviembre 2024
 *
 * Licencia: Código libre bajo MIT. Puede ser modificado y distribuido
 *           bajo los términos establecidos por la licencia.
 ***************************************************************************************/
#include <Arduino.h>
#include "TimerOne.h" // Librería para usar el temporizador 1 de Arduino
#include "DHT.h"      // Librería para el sensor DHT (temperatura y humedad)
#include "PID.hpp"    // Librería personalizada para el controlador PID

// Macros
#define SEMICICLOS_MAX 120 // Número máximo de semiciclos (para el control de encendido del relé)

// Configuración de Pines (GPIO)
#define ZC_PIN 3   // Pin para detectar el cruce por cero (usado para sincronizar el control de encendido)
#define FIRE_PIN 7 // Pin para controlar el disparo del SSR (relé de estado sólido)
#define SENSOR 6   // Pin para conectar el sensor DHT (temperatura y humedad)

/**************************** Parámetros del PID ********************************/
// Parámetros del controlador PID
float KP = 15.0;  // Ganancia proporcional
float KI = 0.25;  // Ganancia integral
float KD = 0.005; // Ganancia derivativa

// Constantes del filtro de paso bajo derivativo
const float TAU = 0.25; // Constante de tiempo para el filtro derivativo

// Tiempo de muestreo del PID (en segundos)
const float T_SAMPLE = 1;

// Límites para la salida del controlador PID
const float LIM_MIN = 0.0;            // Límite mínimo de salida (ciclos ON)
const float LIM_MAX = SEMICICLOS_MAX; // Límite máximo de salida (ciclos ON)

// Límites para el integrador (evitar wind-up)
const float LIM_MIN_INT = -100.00; // Límite inferior para el integrador
const float LIM_MAX_INT = 100.0;   // Límite superior para el integrador

// Punto de consigna (temperatura objetivo)
float SETPOINT = 37.0; // Temperatura deseada (en grados Celsius)

// Instancia del controlador PID
PIDController pid(KP, KI, KD, TAU, LIM_MIN, LIM_MAX, LIM_MIN_INT, LIM_MAX_INT, T_SAMPLE);

/**************************** Variables de control ********************************/
// Variables relacionadas con las interrupciones y el control
volatile bool flagSense = false; // Bandera para indicar que se debe realizar una medición
volatile bool flagZC = false;    // Bandera para indicar que ocurrió un cruce por cero

float temp = 0;         // Variable para almacenar la temperatura medida
uint8_t ciclosOn = 0;   // Número de ciclos ON (controlado por PID)
uint8_t contCiclos = 0; // Contador de ciclos actuales (basado en el cruce por cero)

// Instancia del sensor DHT
DHT sensor; // Inicializar el sensor DHT (se puede cambiar a DHT22 si es necesario)

/**************************** Prototipos de funciones ********************************/
void ZC_ISR();                               // Función de interrupción para el cruce por cero
void SENSE_ISR();                            // Función de interrupción para la lectura del sensor
void parseCommand(String command);           // Función para interpretar los comandos seriales
float parseValue(String command, char type); // Función para extraer un valor de un comando
void printGains();                           // Función para imprimir los valores de las ganancias y el setpoint

void setup()
{
  // Inicializar comunicación serial a 9600 baudios
  Serial.begin(9600);

  // Configuración de pines de entrada y salida
  pinMode(ZC_PIN, INPUT_PULLUP); // Configurar el pin de cruce por cero como entrada con resistencia pull-up
  pinMode(FIRE_PIN, OUTPUT);     // Configurar el pin del SSR como salida

  // Configuración de interrupciones para el cruce por cero
  attachInterrupt(digitalPinToInterrupt(ZC_PIN), ZC_ISR, FALLING); // Detecta el flanco de bajada del cruce por cero

  // Configuración del temporizador 1 para generar interrupciones cada T_SAMPLE segundos
  Timer1.initialize(T_SAMPLE * 1e6); // Configura Timer1 para que se dispare cada T_SAMPLE segundos
  Timer1.attachInterrupt(SENSE_ISR); // Asocia la interrupción para la lectura del sensor con el temporizador

  // Configuración del sensor DHT
  sensor.setup(SENSOR); // Configura el sensor DHT en el pin definido

  // Inicialización de la salida (apagar SSR al inicio)
  digitalWrite(FIRE_PIN, LOW);

  // Mensaje de inicio
  // Serial.println("Controlador PID iniciado.");
  // printGains(); // Imprime los parámetros iniciales del PID
}

void loop()
{
  // Si hay datos disponibles en el puerto serie, se procesan
  if (Serial.available() > 0)
  {
    String incomingCommand = Serial.readStringUntil('\n'); // Lee la línea completa hasta el salto de línea
    parseCommand(incomingCommand);                         // Interpreta el comando recibido
    // printGains();                                          // Imprime los valores actuales del setpoint y las ganancias
  }

  // Si se activó la bandera de sensado, se lee la temperatura y se actualiza el PID
  if (flagSense)
  {
    flagSense = false; // Restablecer la bandera

    temp = sensor.getTemperature(); // Obtener la temperatura del sensor

    // Verificar si la lectura de la temperatura fue exitosa
    if (isnan(temp))
    {
      Serial.println("Error: No se puede leer la temperatura.");
      temp = 0;     // Restablecer temperatura en caso de error
      ciclosOn = 0; // Desactivar ciclos
    }

    // Calcular el número de ciclos ON a partir de la salida del PID
    ciclosOn = static_cast<uint8_t>(pid.update(SETPOINT, temp)); // Actualiza la salida del PID

    // Imprimir la temperatura y el setpoint en el puerto serial
    Serial.print(temp);
    Serial.print(",");
    Serial.println(SETPOINT);
  }

  // Si se detectó un cruce por cero, se maneja el encendido/apagado del SSR
  if (flagZC)
  {
    flagZC = false; // Restablecer la bandera

    // Incrementar el contador de ciclos, y restablecerlo si alcanza el máximo
    if (++contCiclos >= SEMICICLOS_MAX)
    {
      contCiclos = 0;
    }

    // Controlar el estado del SSR (relé) dependiendo de los ciclos ON calculados
    digitalWrite(FIRE_PIN, (contCiclos < ciclosOn) ? HIGH : LOW); // Encender/apagar el SSR
  }
}

// Interrupción para el cruce por cero
void ZC_ISR() { flagZC = true; }

// Interrupción para la lectura del sensor (temporizador)
void SENSE_ISR() { flagSense = true; }

// Función para parsear un valor numérico de un comando
float parseValue(String command, char type)
{
  // Verificar si el comando contiene un valor numérico
  if (command.length() > 1 && isDigit(command[1]))
  {
    return command.substring(1).toFloat(); // Extraer el valor numérico
  }
  else
  {
    Serial.print("Error: valor inválido para ");
    Serial.println(type); // Mensaje de error si no se encuentra un valor válido
    return NAN;           // Retornar "Not-a-Number" si el valor no es válido
  }
}

// Función para interpretar los comandos seriales
void parseCommand(String command)
{
  if (command.startsWith("P")) // Comando para ajustar KP
  {
    float value = parseValue(command, 'P');
    if (!isnan(value))
      KP = value; // Actualizar el valor de KP
  }
  else if (command.startsWith("I")) // Comando para ajustar KI
  {
    float value = parseValue(command, 'I');
    if (!isnan(value))
      KI = value; // Actualizar el valor de KI
  }
  else if (command.startsWith("D")) // Comando para ajustar KD
  {
    float value = parseValue(command, 'D');
    if (!isnan(value))
      KD = value; // Actualizar el valor de KD
  }
  else if (command.startsWith("S")) // Comando para ajustar el setpoint
  {
    float value = parseValue(command, 'S');
    if (!isnan(value))
      SETPOINT = value; // Actualizar el setpoint
  }
  else
  {
    Serial.println("Comando inválido."); // Mensaje de error si el comando no es reconocido
  }

  // Actualizar las ganancias del controlador PID con los nuevos valores
  pid.updateGains(KP, KI, KD);
}

// Función para imprimir los valores actuales de las ganancias y el setpoint
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
