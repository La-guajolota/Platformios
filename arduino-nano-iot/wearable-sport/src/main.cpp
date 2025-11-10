/**
 * @file main.cpp
 * @brief Sistema de monitoreo deportivo con ECG/EMG y GPS
 * @authors Juan Perez, Ana Gomez
 * @date 2025
 * 
 * @details Este programa monitorea en tiempo real la frecuencia cardíaca mediante
 * señales EMG/ECG y la actividad deportiva mediante GPS. Calcula zonas de velocidad,
 * zonas de frecuencia cardíaca, TRIMP, y detecta sprints. Guarda reportes cada minuto
 * en formato CSV en una tarjeta SD.
 */

#include <Arduino.h>
#include <math.h>
#include <SPI.h>
#include <SD.h>
#include "config.h"
#include "filter.h"
#include "heart_rate.h"
#include "gps_processing.h"
#include "velocity_zones.h"
#include "heart_rate_zones.h"
#include "metrics.h"
#include "sd_card.h"

// ==================== CONFIGURACIÓN INICIAL ====================

/**
 * @brief Configuración inicial del sistema
 * 
 * Inicializa comunicaciones seriales, ADC, tarjeta SD y GPS.
 * Crea el archivo CSV si no existe.
 */
void setup() {
  // Comunicación serial con PC
  Serial.begin(BAUD_PC);
  while (!Serial) {
    ; // Esperar conexión serial
  }
  Serial.println(F("=== Sistema de Monitoreo Deportivo ==="));
  
  // Configuración ADC para lectura EMG
  analogReadResolution(ADC_RESOLUTION);
  
  // Inicialización de tarjeta SD
  Serial.print(F("Inicializando SD... "));
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println(F("ERROR: No se pudo inicializar SD"));
    Serial.println(F("Verifique: 1) Tarjeta insertada 2) Conexiones 3) Pin CS correcto"));
  } else {
    Serial.println(F("OK"));
    crearArchivoCSV();
  }
  
  // Comunicación serial con GPS
  Serial1.begin(BAUD_GPS);
  Serial.println(F("GPS inicializado"));
  
  // Inicializar temporizador de minuto
  tMinuteStartMs = millis();
  
  Serial.println(F("Sistema listo\n"));
}

// ==================== BUCLE PRINCIPAL ====================

/**
 * @brief Bucle principal del programa
 * 
 * Ejecuta tres tareas principales:
 * 1. Muestreo y detección de latidos cardíacos a 30 Hz
 * 2. Lectura y procesamiento de datos GPS cada segundo
 * 3. Generación de resumen y guardado en SD cada minuto
 */
void loop() {
  // ==================== PROCESAMIENTO ECG (30 Hz) ====================
  static uint32_t nextEcgUs = 0;
  uint32_t nowUs = micros();
  
  if ((int32_t)(nowUs - nextEcgUs) >= 0) {
    nextEcgUs += 1000000UL / SAMPLE_RATE;
    
    // Lectura y filtrado de señal
    float raw = (float)analogRead(EMG_INPUT_PIN);
    float yf = filterSample(raw);
    
    // Detección de pico local
    s2 = s1;
    s1 = s0;
    s0 = yf;
    bool isLocalMax = (s1 > s2) && (s1 >= s0);
    unsigned long ms = millis();
    
    // Período de warm-up del umbral adaptativo
    if (ms < 1500) {
      noiseLevel = 0.99f * noiseLevel + 0.01f * fabsf(s1);
      thresh = noiseLevel * 1.5f;
    } else {
      // Detección de latido
      if (isLocalMax && (int)(ms - lastBeatMs) > REFRACT_MS) {
        if (s1 > thresh) {
          // Latido válido detectado
          unsigned int rr = ms - lastBeatMs;
          lastBeatMs = ms;
          
          if (rr >= MIN_RR && rr <= MAX_RR) {
            pushRR(rr);
            Serial.print((int)(bpmAvg + 0.5f));
            Serial.println(F(" bpm"));
          }
          signalLevel = 0.875f * signalLevel + 0.125f * s1;
        } else {
          noiseLevel = 0.875f * noiseLevel + 0.125f * fabsf(s1);
        }
        
        // Actualización de umbral adaptativo
        float s = (signalLevel > noiseLevel) ? signalLevel : (noiseLevel + 1.0f);
        thresh = noiseLevel + 0.25f * (s - noiseLevel);
      }
    }
    
    // Recordatorio de BPM cada segundo
    if (ms - lastPrintBpmMs >= 1000) {
      lastPrintBpmMs = ms;
      if (bpmAvg > 0) {
        Serial.print((int)(bpmAvg + 0.5f));
        Serial.println(F(" bpm"));
      } else {
        Serial.println(F("... bpm"));
      }
    }
  }
  
  // ==================== LECTURA GPS ====================
  while (Serial1.available()) {
    gps.encode(Serial1.read());
  }
  
  // ==================== PROCESAMIENTO CADA SEGUNDO ====================
  static uint32_t lastSecMs = 0;
  if (millis() - lastSecMs >= 1000) {
    lastSecMs += 1000;
    
    // Obtener velocidad con filtrado
    float v_kmh = 0.0f;
    if (gps.speed.isValid()) {
      v_kmh = movingAvg(gps.speed.kmph());
      if (v_kmh < V_THRESH_KMH) v_kmh = 0.0f;
    }
    v_kmh_last = v_kmh;
    if (v_kmh > v_kmh_max_min) v_kmh_max_min = v_kmh;
    
    // Cálculo de distancia (v en m/s durante 1 segundo)
    float d_m = v_kmh / 3.6f;
    dist_m_total += d_m;
    dist_m_minute += d_m;
    
    // Acumulación en zona de velocidad
    int vz = velZoneIndex(v_kmh);
    secInVZ[vz] += 1.0f;
    distInVZ[vz] += d_m;
    
    // Acumulación en zona de frecuencia cardíaca
    float thisBpm = bpmAvg;
    int hz = hrZoneIndex(thisBpm);
    secInHZ[hz] += 1.0f;
    if (thisBpm > 0) {
      bpmSumSec += thisBpm;
      bpmSecCount++;
    }
    
    // Cálculo de TRIMP (por minuto, dividido entre 60)
    trimpMinute += TRIMP_W[hz] * (1.0f / 60.0f);
    
    // Detección de sprints (requiere 2 segundos consecutivos)
    if (v_kmh >= SPRINT_KMH) {
      sprintHold_s++;
      if (!inSprint && sprintHold_s >= 2) {
        inSprint = true;
        sprints_min++;
        sprints_total++;
      }
    } else {
      sprintHold_s = 0;
      inSprint = false;
    }
    
    // ==================== RESUMEN CADA MINUTO ====================
    if (millis() - tMinuteStartMs >= 60000) {
      tMinuteStartMs += 60000;
      
      // Cálculo de promedios
      float bpmMeanMin = (bpmSecCount > 0) ? bpmSumSec / bpmSecCount : 0.0f;
      float vMeanMin = dist_m_minute / 60.0f * 3.6f;
      
      // Impresión en serial
      Serial.println(F("\n===== RESUMEN (1 min) ====="));
      Serial.print(F("Dist: "));
      Serial.print(dist_m_minute, 1);
      Serial.print(F(" m  (Total: "));
      Serial.print(dist_m_total / 1000.0f, 3);
      Serial.println(F(" km)"));
      
      Serial.print(F("Vel prom: "));
      Serial.print(vMeanMin, 1);
      Serial.print(F(" km/h  Vel pico: "));
      Serial.print(v_kmh_max_min, 1);
      Serial.println(F(" km/h"));
      
      Serial.print(F("BPM prom: "));
      Serial.println(bpmMeanMin, 1);
      
      Serial.println(F("Zonas vel [s | m]: CAM/TRO/CAR/SPR"));
      Serial.print((int)secInVZ[VZ_CAM]);
      Serial.print(F("s | "));
      Serial.print((int)distInVZ[VZ_CAM]);
      Serial.println(F("m"));
      Serial.print((int)secInVZ[VZ_TRO]);
      Serial.print(F("s | "));
      Serial.print((int)distInVZ[VZ_TRO]);
      Serial.println(F("m"));
      Serial.print((int)secInVZ[VZ_CAR]);
      Serial.print(F("s | "));
      Serial.print((int)distInVZ[VZ_CAR]);
      Serial.println(F("m"));
      Serial.print((int)secInVZ[VZ_SPR]);
      Serial.print(F("s | "));
      Serial.print((int)distInVZ[VZ_SPR]);
      Serial.println(F("m"));
      
      Serial.println(F("Zonas FC [s]: Z1<50%, Z2<60, Z3<70, Z4<80, Z5<90, Z6>=90"));
      Serial.print((int)secInHZ[HZ1]); Serial.print(' ');
      Serial.print((int)secInHZ[HZ2]); Serial.print(' ');
      Serial.print((int)secInHZ[HZ3]); Serial.print(' ');
      Serial.print((int)secInHZ[HZ4]); Serial.print(' ');
      Serial.print((int)secInHZ[HZ5]); Serial.print(' ');
      Serial.println((int)secInHZ[HZ6]);
      
      Serial.print(F("TRIMP (min): "));
      Serial.println(trimpMinute, 2);
      Serial.print(F("Sprints(min/total): "));
      Serial.print(sprints_min);
      Serial.print('/');
      Serial.println(sprints_total);
      Serial.println(F("===========================\n"));
      
      // Guardar datos en tarjeta SD
      guardarDatosCSV(bpmMeanMin, vMeanMin);
      
      // Reiniciar acumuladores
      resetMinuteAccumulators();
    }
  }
  
  // ==================== REPORTE GPS PERIÓDICO ====================
  static uint32_t tGps = 0;
  if (millis() - tGps >= 1000) {
    tGps = millis();
    
    // Posición
    if (gps.location.isValid()) {
      double lat = gps.location.lat();
      double lon = gps.location.lng();
      Serial.print(F("Lat: "));
      Serial.print(lat, 6);
      Serial.print(F("  Lon: "));
      Serial.println(lon, 6);
    } else {
      Serial.println(F("Posición: buscando fix..."));
    }
    
    // Fecha y hora
    if (gps.date.isValid() && gps.time.isValid()) {
      int y = gps.date.year();
      int m = gps.date.month();
      int d = gps.date.day();
      int uh = gps.time.hour();
      int um = gps.time.minute();
      int us = gps.time.second();
      int lh = wrapLocalHour(uh, UTC_OFFSET_H);
      
      Serial.print(F("UTC: "));
      Serial.print(y); Serial.print('-');
      Serial.print(m); Serial.print('-');
      Serial.print(d); Serial.print(' ');
      Serial.print(uh); Serial.print(':');
      Serial.print(um); Serial.print(':');
      Serial.println(us);
      
      Serial.print(F("Local: "));
      Serial.print(lh); Serial.print(':');
      Serial.print(um); Serial.print(':');
      Serial.println(us);
    }
    
    // Satélites y precisión
    if (gps.satellites.isValid()) {
      Serial.print(F("Sats: "));
      Serial.print(gps.satellites.value());
      Serial.print(F("  HDOP: "));
      if (gps.hdop.isValid()) {
        Serial.println(gps.hdop.hdop());
      } else {
        Serial.println(F("N/D"));
      }
    }
    
    // Altitud
    if (gps.altitude.isValid()) {
      Serial.print(F("Altitud: "));
      Serial.print(gps.altitude.meters());
      Serial.println(F(" m"));
    }
    
    // Velocidad
    if (gps.speed.isValid()) {
      Serial.print(F("Vel (km/h): "));
      Serial.println(v_kmh_last, 2);
    } else {
      Serial.println(F("Velocidad: N/D"));
    }
    
    Serial.println();
  }
}