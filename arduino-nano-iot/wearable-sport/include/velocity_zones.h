/**
 * @file velocity_zones.h
 * @brief Gestión de zonas de velocidad
 * @authors Juan Perez, Ana Gomez
 * @date 2025
 */

#ifndef VELOCITY_ZONES_H
#define VELOCITY_ZONES_H

#include "config.h"

// ==================== ZONAS DE VELOCIDAD ====================

/** @brief Índices de zonas de velocidad */
enum VelZone { 
  VZ_CAM = 0,  ///< Caminando
  VZ_TRO = 1,  ///< Trotando
  VZ_CAR = 2,  ///< Corriendo
  VZ_SPR = 3   ///< Sprint
};

extern float secInVZ[4];                 ///< Segundos acumulados por zona de velocidad
extern float distInVZ[4];                ///< Distancia acumulada por zona de velocidad (m)

/**
 * @brief Determina la zona de velocidad según la velocidad actual
 * 
 * @param v Velocidad en km/h
 * @return int Índice de zona (0-3)
 */
int velZoneIndex(float v);

#endif // VELOCITY_ZONES_H
