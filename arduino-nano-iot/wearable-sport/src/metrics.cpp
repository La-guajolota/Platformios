/**
 * @file metrics.cpp
 * @brief Implementación de métricas de entrenamiento
 * @authors Juan Perez, Ana Gomez
 * @date 2025
 */

#include <stdint.h>
#include "metrics.h"

// ==================== CONSTANTES ====================

const float TRIMP_W[] = {1, 2, 3, 4, 5, 6};
const float SPRINT_KMH = 20.0;

// ==================== VARIABLES ====================

float trimpMinute = 0;
float dist_m_total = 0;
float dist_m_minute = 0;
float v_kmh_last = 0;
float v_kmh_max_min = 0;

int sprints_min = 0;
int sprints_total = 0;
bool inSprint = false;
int sprintHold_s = 0;

uint32_t lastGpsMs = 0;
uint32_t tMinuteStartMs = 0;
