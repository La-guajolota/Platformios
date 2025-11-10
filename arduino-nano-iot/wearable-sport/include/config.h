/**
 * @file config.h
 * @brief Configuraciones y constantes del sistema
 * @authors Juan Perez, Ana Gomez
 * @date 2025
 */

#ifndef CONFIG_H
#define CONFIG_H

// ==================== CONFIGURACIONES DE HARDWARE ====================

/** @brief Velocidad de comunicación serial con PC */
#define BAUD_PC           115200

/** @brief Pin CS (Chip Select) de la tarjeta SD */
#define SD_CS_PIN         10

/** @brief Pin analógico para lectura de señal EMG/ECG */
#define EMG_INPUT_PIN     PIN_A0

/** @brief Frecuencia de muestreo para señal ECG (Hz) */
#define SAMPLE_RATE       30

/** @brief Velocidad de comunicación serial con módulo GPS */
#define BAUD_GPS          9600

/** @brief Offset de zona horaria en horas (México: -6) */
#define UTC_OFFSET_H      -6

/** @brief Nombre del archivo CSV para guardar datos */
#define CSV_FILENAME      "datos.csv"

// ==================== CONSTANTES DE DETECCIÓN CARDÍACA ====================

/** @brief Período refractario en ms (~200 bpm máximo) */
extern const int REFRACT_MS;

/** @brief Rango válido de intervalos RR en ms (30-200 bpm) */
extern const int MIN_RR;
extern const int MAX_RR;

/** @brief Tamaño del buffer para promedio de intervalos RR */
#define RR_BUF 4

// ==================== CONSTANTES GPS ====================

/** @brief Tamaño de ventana para promedio móvil de velocidad */
extern const int V_WIN;

/** @brief Umbral de velocidad mínima para considerar movimiento (km/h) */
extern const float V_THRESH_KMH;

// ==================== CONSTANTES DE ZONAS ====================

/** @brief Umbrales de velocidad para zonas (km/h): caminar/trotar/correr/sprint */
extern const float V_BINS[3];

/** @brief Frecuencia cardíaca máxima de referencia (bpm) */
extern const int FCMAX;

/** @brief Bordes de zonas de FC como porcentaje de FC máxima */
extern const float Z_EDGES[5];

/** @brief Pesos para cálculo de TRIMP por zona de FC */
extern const float TRIMP_W[6];

/** @brief Umbral de velocidad para considerar sprint (km/h) */
extern const float SPRINT_KMH;

#endif // CONFIG_H
