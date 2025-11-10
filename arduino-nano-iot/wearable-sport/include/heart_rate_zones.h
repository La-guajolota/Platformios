/**
 * @file heart_rate_zones.h
 * @brief Gestión de zonas de frecuencia cardíaca
 * @authors Juan Perez, Ana Gomez
 * @date 2025
 */

#ifndef HEART_RATE_ZONES_H
#define HEART_RATE_ZONES_H

#include "config.h"

// ==================== ZONAS DE FRECUENCIA CARDÍACA ====================

/** @brief Índices de zonas de frecuencia cardíaca */
enum HRZone {
  HZ1 = 0,  ///< Zona 1: < 50% FC máx
  HZ2 = 1,  ///< Zona 2: 50-60% FC máx
  HZ3 = 2,  ///< Zona 3: 60-70% FC máx
  HZ4 = 3,  ///< Zona 4: 70-80% FC máx
  HZ5 = 4,  ///< Zona 5: 80-90% FC máx
  HZ6 = 5   ///< Zona 6: >= 90% FC máx
};

extern float secInHZ[6];                 ///< Segundos acumulados por zona de FC
extern float bpmSumSec;                  ///< Suma de BPM para promedio
extern int bpmSecCount;                  ///< Contador de muestras para promedio BPM

/**
 * @brief Determina la zona de frecuencia cardíaca según BPM actual
 * 
 * @param bpm Frecuencia cardíaca en latidos por minuto
 * @return int Índice de zona (0-5)
 */
int hrZoneIndex(float bpm);

#endif // HEART_RATE_ZONES_H
