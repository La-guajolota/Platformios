/**
 * @file filter.cpp
 * @brief Implementación del filtrado de señal ECG
 * @authors Juan Perez, Ana Gomez
 * @date 2025
 */

#include "filter.h"

/**
 * @brief Banco de filtros biquad en cascada para filtrado de señal ECG
 * 
 * Configuración de 4 etapas de filtros para eliminar ruido y artefactos
 * de la señal cardíaca
 */
Biquad sos[] = {
  {0.37373487f,  0.74746975f,  0.37373487f,   1.02830960f,  0.28774614f, 0, 0},
  {1.00000000f,  2.00000000f,  1.00000000f,   1.32440218f,  0.64037407f, 0, 0},
  {1.00000000f, -2.00000000f,  1.00000000f,  -1.80939660f,  0.81997336f, 0, 0},
  {1.00000000f, -2.00000000f,  1.00000000f,  -1.91424301f,  0.92488759f, 0, 0}
};

/**
 * @brief Aplica el banco de filtros biquad a una muestra
 * 
 * @param x Muestra de entrada sin filtrar
 * @return float Muestra filtrada
 */
float filterSample(float x) {
  for (int i = 0; i < (int)(sizeof(sos) / sizeof(sos[0])); ++i) {
    Biquad &s = sos[i];
    float y = s.b0 * x + s.z1;
    s.z1    = s.b1 * x - s.a1 * y + s.z2;
    s.z2    = s.b2 * x - s.a2 * y;
    x = y;
  }
  return x;
}
