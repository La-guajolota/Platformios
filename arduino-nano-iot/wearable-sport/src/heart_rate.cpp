/**
 * @file heart_rate.cpp
 * @brief Implementación de detección de frecuencia cardíaca
 * @authors Juan Perez, Ana Gomez
 * @date 2025
 */

#include "heart_rate.h"

// ==================== CONSTANTES ====================

const int REFRACT_MS = 300;
const int MIN_RR = 300;
const int MAX_RR = 2000;

// ==================== VARIABLES DE DETECCIÓN CARDÍACA ====================

float s0 = 0, s1 = 0, s2 = 0;
float signalLevel = 0;
float noiseLevel = 0;
float thresh = 0;
unsigned long lastBeatMs = 0;
unsigned long lastPrintBpmMs = 0;
float bpmAvg = 0;

unsigned int rrBuf[RR_BUF];
int rrIdx = 0;
bool rrFilled = false;

/**
 * @brief Agrega un intervalo RR al buffer y recalcula BPM promedio
 * 
 * @param rr Intervalo RR en milisegundos
 */
void pushRR(unsigned int rr) {
  rrBuf[rrIdx++] = rr;
  if (rrIdx >= RR_BUF) {
    rrIdx = 0;
    rrFilled = true;
  }
  
  long sum = 0;
  int n = rrFilled ? RR_BUF : rrIdx;
  for (int i = 0; i < n; i++) {
    sum += rrBuf[i];
  }
  
  float rrMean = (n > 0) ? (float)sum / n : rr;
  bpmAvg = 60000.0f / rrMean;
}
