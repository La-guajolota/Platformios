/****************************************************************************************
 * Archivo: PID.hpp
 * Descripción: Este archivo contiene la declaración de la clase `PIDController` que
 *              implementa un controlador PID para sistemas embebidos. El controlador
 *              incluye los cálculos de los términos proporcional, integral y derivativo
 *              y tiene soporte para la actualización dinámica de las ganancias, así como
 *              protección contra el *wind-up* del integrador.
 *
 * Autor: Adrian Silva Palafox
 * Empresa: Fourie Embeds
 * Fecha de creación: Noviembre 2024
 *
 * Licencia: Este código es de código abierto bajo la licencia [Tu Licencia Aquí].
 *           Puede ser modificado y distribuido con fines educativos o comerciales.
 *
 * Nota: Asegúrese de que las librerías necesarias estén correctamente incluidas y
 *       configuradas en su entorno de desarrollo.
 ***************************************************************************************/

#ifndef PID_HPP
#define PID_HPP

// Estructura para agrupar las ganancias del controlador PID
// Esta estructura es opcional y permite retornar las tres ganancias (Kp, Ki, Kd) en un solo objeto
struct PIDGains
{
    float Kp; // Ganancia proporcional
    float Ki; // Ganancia integral
    float Kd; // Ganancia derivativa
};

class PIDController
{
public:
    // Constructor de la clase PIDController
    // Inicializa los parámetros del PID, como las ganancias y los límites de salida e integrador.
    PIDController(float kp, float ki, float kd,
                  float tau,                        // Constante del filtro de paso bajo para el derivativo
                  float limMin, float limMax,       // Límites de salida del controlador
                  float limMinInt, float limMaxInt, // Límites del integrador para evitar el wind-up
                  float t);                         // Tiempo de muestreo (en segundos)

    // Métodos públicos:

    // Reinicia el controlador PID a su estado inicial (borra el integrador, el error previo, etc.)
    void reset();

    // Actualiza la salida del controlador PID con base en el setpoint (punto de consigna) y la medición actual
    float update(float setpoint, float measurement);

    // Actualiza las ganancias del controlador PID (Kp, Ki, Kd) en tiempo real
    void updateGains(float kp, float ki, float kd);

    // Métodos para obtener las ganancias actuales del PID:
    float getKp() const; // Retorna la ganancia proporcional (Kp)
    float getKi() const; // Retorna la ganancia integral (Ki)
    float getKd() const; // Retorna la ganancia derivativa (Kd)

    // Método para obtener todas las ganancias juntas como un struct
    // Esto es útil para enviar todas las ganancias en un solo paquete, por ejemplo, por comunicación serial
    PIDGains getGains() const;

private:
    // Parámetros internos del controlador PID
    float Kp; // Ganancia proporcional
    float Ki; // Ganancia integral
    float Kd; // Ganancia derivativa

    float tau; // Constante del filtro de paso bajo para el término derivativo

    // Límites de salida del controlador
    float limMin; // Límite mínimo de la salida
    float limMax; // Límite máximo de la salida

    // Límites para el integrador (para evitar el wind-up)
    float limMinInt; // Límite mínimo para el integrador
    float limMaxInt; // Límite máximo para el integrador

    // Tiempo de muestreo (en segundos)
    float T;

    // Variables de memoria interna del controlador
    float integrator;      // Acumulador del término integral
    float prevError;       // Error de la iteración anterior (necesario para calcular el término derivativo)
    float differentiator;  // Almacena el valor del término derivativo
    float prevMeasurement; // Medición anterior (necesaria para calcular el derivado)

    // Salida del controlador (resultado final después de aplicar la fórmula PID)
    float out;
};

#endif // PID_HPP
