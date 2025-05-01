/****************************************************************************************
 * Archivo: PID.hpp
 * Descripción: Implementación de un controlador PID para sistemas embebidos, utilizado
 *              en la regulación de sistemas de temperatura o variables físicas.
 *              Este archivo contiene la clase `PIDController` que gestiona los cálculos
 *              PID, con soporte para ajustes dinámicos de ganancias y limitación de la
 *              salida del controlador.
 *
 * Autor: Adrian Silva Palafox
 * Empresa: Fourie Embeds
 * Fecha de creación: Noviembre 2024
 *
 * Licencia: Este código es de código abierto bajo la licencia [Tu Licencia Aquí].
 *           Puede ser modificado y distribuido con fines educativos o comerciales.
 *
 * Nota: Se asume que las librerías necesarias como "PID.hpp" y otras dependencias ya
 *       están correctamente incluidas y configuradas en el entorno de desarrollo.
 ***************************************************************************************/

// Inclusión del archivo de cabecera para la implementación del PID
#include "PID.hpp"

// Constructor de la clase PIDController
PIDController::PIDController(float kp, float ki, float kd,
                             float tau,
                             float limMin, float limMax,
                             float limMinInt, float limMaxInt,
                             float t)
    : Kp(kp), Ki(ki), Kd(kd),                     // Inicialización de las ganancias PID (proporcional, integral, derivativa)
      tau(tau),                                   // Tiempo de constante del filtro de paso bajo (para el derivativo)
      limMin(limMin), limMax(limMax),             // Límites para la salida del controlador
      limMinInt(limMinInt), limMaxInt(limMaxInt), // Límites para el integrador (prevención de wind-up)
      T(t),                                       // Tiempo de muestreo (intervalo de tiempo entre actualizaciones)
      integrator(0.0f),                           // Inicialización del integrador (acumula el error)
      prevError(0.0f),                            // Inicialización del error previo (para el cálculo del derivativo)
      differentiator(0.0f),                       // Inicialización del diferenciador (para el término derivativo)
      prevMeasurement(0.0f),                      // Inicialización de la medición previa (para el cálculo del derivativo)
      out(0.0f)                                   // Inicialización de la salida del controlador
{
}

// Método para reiniciar el controlador PID
void PIDController::reset()
{
    integrator = 0.0f;      // Reiniciar el integrador
    prevError = 0.0f;       // Reiniciar el error previo
    differentiator = 0.0f;  // Reiniciar el diferenciador
    prevMeasurement = 0.0f; // Reiniciar la medición previa
    out = 0.0f;             // Reiniciar la salida
}

// Método que actualiza el controlador PID cada vez que se llama
float PIDController::update(float setpoint, float measurement)
{
    // Calcular el error (diferencia entre el setpoint y la medición)
    float error = setpoint - measurement;

    // Cálculo del término proporcional (Kp * error)
    float proportional = Kp * error;

    // Cálculo del término integral (integración del error)
    integrator += 0.5f * Ki * T * (error + prevError); // Método de integración trapezoidal (suma promedio de los errores)

    // Anti-windup: Limitar el valor del integrador para evitar que se acumule demasiado error
    if (integrator > limMaxInt)
    {
        integrator = limMaxInt; // Limitar el integrador al valor máximo permitido
    }
    else if (integrator < limMinInt)
    {
        integrator = limMinInt; // Limitar el integrador al valor mínimo permitido
    }

    // Cálculo del término derivativo (con filtro de paso bajo para evitar ruido)
    differentiator = -(2.0f * Kd * (measurement - prevMeasurement) +
                       (2.0f * tau - T) * differentiator) /
                     (2.0f * tau + T);

    // Cálculo de la salida total del controlador (Suma de proporcional, integral y derivativo)
    out = proportional + integrator + differentiator;

    // Aplicar límites a la salida (salida del controlador debe estar dentro de los límites definidos)
    if (out > limMax)
    {
        out = limMax; // Limitar la salida al máximo
    }
    else if (out < limMin)
    {
        out = limMin; // Limitar la salida al mínimo
    }

    // Actualizar la memoria del controlador (errores y mediciones previas para el próximo ciclo)
    prevError = error;
    prevMeasurement = measurement;

    // Retornar la salida del controlador (cuánto debe ajustarse la variable controlada)
    return out;
}

// Método para actualizar las ganancias Kp, Ki y Kd en tiempo de ejecución
void PIDController::updateGains(float kp, float ki, float kd)
{
    Kp = kp; // Actualizar la ganancia proporcional
    Ki = ki; // Actualizar la ganancia integral
    Kd = kd; // Actualizar la ganancia derivativa
}

// Métodos para obtener las ganancias Kp, Ki y Kd de forma individual
float PIDController::getKp() const
{
    return Kp; // Retornar la ganancia proporcional
}

float PIDController::getKi() const
{
    return Ki; // Retornar la ganancia integral
}

float PIDController::getKd() const
{
    return Kd; // Retornar la ganancia derivativa
}

// Método para obtener todas las ganancias juntas como un struct (útil para enviar por serial u otros)
PIDGains PIDController::getGains() const
{
    return {Kp, Ki, Kd}; // Retornar las tres ganancias como un struct
}
