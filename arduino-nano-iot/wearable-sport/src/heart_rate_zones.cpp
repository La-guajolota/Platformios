/**
 * @file heart_rate_zones.cpp
 * @brief Implementación de zonas de frecuencia cardíaca
 * @authors Juan Perez, Ana Gomez
 * @date 2025
 */

#include "heart_rate_zones.h"

// ==================== CONSTANTES ====================

const int FCMAX = 200;
const float Z_EDGES[] = {0.50, 0.60, 0.70, 0.80, 0.90};

// ==================== VARIABLES ====================

float secInHZ[6] = {0};
float bpmSumSec = 0;
int bpmSecCount = 0;

/**
 * @brief Determina la zona de frecuencia cardíaca según BPM actual
 * 
 * @param bpm Frecuencia cardíaca en latidos por minuto
 * @return int Índice de zona (0-5)
 */
int hrZoneIndex(float bpm) {
  if (bpm <= 0) return HZ1;
  float r = bpm / FCMAX;
  if (r < Z_EDGES[0]) return HZ1;
  if (r < Z_EDGES[1]) return HZ2;
  if (r < Z_EDGES[2]) return HZ3;
  if (r < Z_EDGES[3]) return HZ4;
  if (r < Z_EDGES[4]) return HZ5;
  return HZ6;
}
