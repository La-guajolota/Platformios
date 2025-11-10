/**
 * @file velocity_zones.cpp
 * @brief Implementación de zonas de velocidad
 * @authors Juan Perez, Ana Gomez
 * @date 2025
 */

#include "velocity_zones.h"

// ==================== CONSTANTES ====================

const float V_BINS[] = {7.0, 15.0, 20.0};

// ==================== VARIABLES ====================

float secInVZ[4] = {0};
float distInVZ[4] = {0};

/**
 * @brief Determina la zona de velocidad según la velocidad actual
 * 
 * @param v Velocidad en km/h
 * @return int Índice de zona (0-3)
 */
int velZoneIndex(float v) {
  if (v < V_BINS[0]) return VZ_CAM;
  if (v < V_BINS[1]) return VZ_TRO;
  if (v < V_BINS[2]) return VZ_CAR;
  return VZ_SPR;
}
