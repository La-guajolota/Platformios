/**
 * @file heart_rate.h
 * @brief Detección de frecuencia cardíaca mediante señal ECG/EMG
 * @authors Juan Perez, Ana Gomez
 * @date 2025
 */

#ifndef HEART_RATE_H
#define HEART_RATE_H

#include "config.h"

// ==================== VARIABLES DE DETECCIÓN CARDÍACA ====================

extern float s0, s1, s2;                 ///< Buffer para detección de picos locales
extern float signalLevel;                ///< Nivel promedio de señal
extern float noiseLevel;                 ///< Nivel promedio de ruido
extern float thresh;                     ///< Umbral adaptativo para detección
extern unsigned long lastBeatMs;         ///< Timestamp del último latido detectado
extern unsigned long lastPrintBpmMs;     ///< Timestamp de última impresión de BPM
extern float bpmAvg;                     ///< BPM promedio actual

extern unsigned int rrBuf[RR_BUF];       ///< Buffer circular de intervalos RR
extern int rrIdx;                        ///< Índice actual en buffer RR
extern bool rrFilled;                    ///< Indica si el buffer RR está lleno

/**
 * @brief Agrega un intervalo RR al buffer y recalcula BPM promedio
 * 
 * @param rr Intervalo RR en milisegundos
 */
void pushRR(unsigned int rr);

#endif // HEART_RATE_H
