/**
 * @file metrics.h
 * @brief Métricas de entrenamiento y detección de sprints
 * @authors Juan Perez, Ana Gomez
 * @date 2025
 */

#ifndef METRICS_H
#define METRICS_H

#include "config.h"

// ==================== MÉTRICAS DE ENTRENAMIENTO ====================

extern float trimpMinute;                ///< TRIMP acumulado en el minuto actual
extern float dist_m_total;               ///< Distancia total acumulada (m)
extern float dist_m_minute;              ///< Distancia del minuto actual (m)
extern float v_kmh_last;                 ///< Última velocidad válida (km/h)
extern float v_kmh_max_min;              ///< Velocidad máxima del minuto (km/h)

// ==================== DETECCIÓN DE SPRINTS ====================

extern int sprints_min;                  ///< Sprints detectados en el minuto actual
extern int sprints_total;                ///< Sprints totales acumulados
extern bool inSprint;                    ///< Indica si está en sprint actualmente
extern int sprintHold_s;                 ///< Segundos consecutivos sobre umbral de sprint

// ==================== VARIABLES DE TEMPORIZACIÓN ====================

extern uint32_t lastGpsMs;               ///< Timestamp de última lectura GPS
extern uint32_t tMinuteStartMs;          ///< Timestamp de inicio del minuto actual

#endif // METRICS_H
