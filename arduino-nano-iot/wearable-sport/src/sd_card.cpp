/**
 * @file sd_card.cpp
 * @brief Implementación de funciones para tarjeta SD
 * @authors Juan Perez, Ana Gomez
 * @date 2025
 */

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "sd_card.h"
#include "config.h"
#include "gps_processing.h"
#include "velocity_zones.h"
#include "heart_rate_zones.h"
#include "metrics.h"

/**
 * @brief Crea el archivo CSV con encabezados si no existe
 * 
 * Crea el archivo CSV en la tarjeta SD e inserta la línea de encabezados
 * con los nombres de todas las columnas.
 */
void crearArchivoCSV() {
  if (!SD.exists(CSV_FILENAME)) {
    File dataFile = SD.open(CSV_FILENAME, FILE_WRITE);
    if (dataFile) {
      // Escribir encabezados
      dataFile.println(F("Timestamp,Dist_m,Dist_Total_km,Vel_Prom_kmh,Vel_Max_kmh,"
                         "BPM_Prom,Seg_CAM,Seg_TRO,Seg_CAR,Seg_SPR,"
                         "Dist_CAM_m,Dist_TRO_m,Dist_CAR_m,Dist_SPR_m,"
                         "Seg_HZ1,Seg_HZ2,Seg_HZ3,Seg_HZ4,Seg_HZ5,Seg_HZ6,"
                         "TRIMP,Sprints_Min,Sprints_Total"));
      dataFile.close();
      Serial.println(F("Archivo CSV creado con encabezados"));
    } else {
      Serial.println(F("Error al crear archivo CSV"));
    }
  }
}

/**
 * @brief Guarda los datos del resumen del minuto en el archivo CSV
 * 
 * @param bpmMeanMin BPM promedio del minuto
 * @param vMeanMin Velocidad promedio del minuto (km/h)
 */
void guardarDatosCSV(float bpmMeanMin, float vMeanMin) {
  File dataFile = SD.open(CSV_FILENAME, FILE_WRITE);
  
  if (dataFile) {
    // Timestamp (si GPS disponible)
    if (gps.date.isValid() && gps.time.isValid()) {
      dataFile.print(gps.date.year());
      dataFile.print('-');
      if (gps.date.month() < 10) dataFile.print('0');
      dataFile.print(gps.date.month());
      dataFile.print('-');
      if (gps.date.day() < 10) dataFile.print('0');
      dataFile.print(gps.date.day());
      dataFile.print(' ');
      
      int lh = wrapLocalHour(gps.time.hour(), UTC_OFFSET_H);
      if (lh < 10) dataFile.print('0');
      dataFile.print(lh);
      dataFile.print(':');
      if (gps.time.minute() < 10) dataFile.print('0');
      dataFile.print(gps.time.minute());
      dataFile.print(':');
      if (gps.time.second() < 10) dataFile.print('0');
      dataFile.print(gps.time.second());
    } else {
      dataFile.print(millis() / 1000); // Usar millis como fallback
    }
    dataFile.print(',');
    
    // Distancias
    dataFile.print(dist_m_minute, 1);
    dataFile.print(',');
    dataFile.print(dist_m_total / 1000.0f, 3);
    dataFile.print(',');
    
    // Velocidades
    dataFile.print(vMeanMin, 1);
    dataFile.print(',');
    dataFile.print(v_kmh_max_min, 1);
    dataFile.print(',');
    
    // BPM
    dataFile.print(bpmMeanMin, 1);
    dataFile.print(',');
    
    // Zonas de velocidad - tiempo
    for (int i = 0; i < 4; i++) {
      dataFile.print((int)secInVZ[i]);
      dataFile.print(',');
    }
    
    // Zonas de velocidad - distancia
    for (int i = 0; i < 4; i++) {
      dataFile.print((int)distInVZ[i]);
      dataFile.print(',');
    }
    
    // Zonas de frecuencia cardíaca
    for (int i = 0; i < 6; i++) {
      dataFile.print((int)secInHZ[i]);
      if (i < 5) dataFile.print(',');
    }
    dataFile.print(',');
    
    // TRIMP y Sprints
    dataFile.print(trimpMinute, 2);
    dataFile.print(',');
    dataFile.print(sprints_min);
    dataFile.print(',');
    dataFile.println(sprints_total);
    
    dataFile.close();
    Serial.println(F("Datos guardados en SD"));
  } else {
    Serial.println(F("Error al abrir archivo CSV"));
  }
}

/**
 * @brief Reinicia todos los acumuladores del minuto actual
 */
void resetMinuteAccumulators() {
  for (int i = 0; i < 4; i++) {
    secInVZ[i] = 0;
    distInVZ[i] = 0;
  }
  for (int i = 0; i < 6; i++) {
    secInHZ[i] = 0;
  }
  trimpMinute = 0;
  dist_m_minute = 0;
  v_kmh_max_min = 0;
  sprints_min = 0;
  bpmSumSec = 0;
  bpmSecCount = 0;
  sprintHold_s = 0;
  inSprint = false;
}
