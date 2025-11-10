/**
 * @file gps_processing.h
 * @brief Procesamiento de datos GPS
 * @authors Juan Perez, Ana Gomez
 * @date 2025
 */

#ifndef GPS_PROCESSING_H
#define GPS_PROCESSING_H

#include <TinyGPSPlus.h>
#include "config.h"

// ==================== VARIABLES GPS ====================

extern TinyGPSPlus gps;                  ///< Objeto para procesamiento de datos GPS

extern float vbuf[5];                    ///< Buffer para promedio móvil de velocidad
extern int vidx;                         ///< Índice en buffer de velocidad
extern int vfilled;                      ///< Contador de llenado de buffer

/**
 * @brief Calcula el promedio móvil de velocidad
 * 
 * @param v Velocidad actual en km/h
 * @return float Velocidad promediada
 */
float movingAvg(float v);

/**
 * @brief Ajusta la hora UTC a hora local
 * 
 * @param utcHour Hora en UTC (0-23)
 * @param offset Offset de zona horaria en horas
 * @return int Hora local (0-23)
 */
int wrapLocalHour(int utcHour, int offset);

#endif // GPS_PROCESSING_H
