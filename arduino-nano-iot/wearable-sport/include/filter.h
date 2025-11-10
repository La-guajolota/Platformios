/**
 * @file filter.h
 * @brief Filtrado de señal ECG mediante filtros biquad
 * @authors Juan Perez, Ana Gomez
 * @date 2025
 */

#ifndef FILTER_H
#define FILTER_H

/**
 * @struct Biquad
 * @brief Filtro biquad de segundo orden para procesamiento de señal
 * 
 * Implementa un filtro IIR de segundo orden con ecuación:
 * y[n] = b0*x[n] + b1*x[n-1] + b2*x[n-2] - a1*y[n-1] - a2*y[n-2]
 */
struct Biquad {
  float b0, b1, b2;  ///< Coeficientes del numerador
  float a1, a2;      ///< Coeficientes del denominador
  float z1, z2;      ///< Estados internos del filtro
};

/**
 * @brief Banco de filtros biquad en cascada para filtrado de señal ECG
 */
extern Biquad sos[4];

/**
 * @brief Aplica el banco de filtros biquad a una muestra
 * 
 * @param x Muestra de entrada sin filtrar
 * @return float Muestra filtrada
 */
float filterSample(float x);

#endif // FILTER_H
