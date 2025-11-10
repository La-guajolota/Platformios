/**
 * @file gps_processing.cpp
 * @brief Implementación del procesamiento de datos GPS
 * @authors Juan Perez, Ana Gomez
 * @date 2025
 */

#include "gps_processing.h"

// ==================== CONSTANTES ====================

const int V_WIN = 5;
const float V_THRESH_KMH = 0.7;

// ==================== VARIABLES GPS ====================

TinyGPSPlus gps;

float vbuf[5] = {0};
int vidx = 0;
int vfilled = 0;

/**
 * @brief Calcula el promedio móvil de velocidad
 * 
 * @param v Velocidad actual en km/h
 * @return float Velocidad promediada
 */
float movingAvg(float v) {
  vbuf[vidx] = v;
  vidx = (vidx + 1) % V_WIN;
  if (vfilled < V_WIN) vfilled++;
  
  float s = 0;
  for (int i = 0; i < vfilled; i++) {
    s += vbuf[i];
  }
  return s / vfilled;
}

/**
 * @brief Ajusta la hora UTC a hora local
 * 
 * @param utcHour Hora en UTC (0-23)
 * @param offset Offset de zona horaria en horas
 * @return int Hora local (0-23)
 */
int wrapLocalHour(int utcHour, int offset) {
  int h = utcHour + offset;
  while (h < 0)  h += 24;
  while (h >= 24) h -= 24;
  return h;
}
