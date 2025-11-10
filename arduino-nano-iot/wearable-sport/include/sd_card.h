/**
 * @file sd_card.h
 * @brief Funciones para gestión de tarjeta SD
 * @authors Juan Perez, Ana Gomez
 * @date 2025
 */

#ifndef SD_CARD_H
#define SD_CARD_H

/**
 * @brief Crea el archivo CSV con encabezados si no existe
 * 
 * Crea el archivo CSV en la tarjeta SD e inserta la línea de encabezados
 * con los nombres de todas las columnas.
 */
void crearArchivoCSV();

/**
 * @brief Guarda los datos del resumen del minuto en el archivo CSV
 * 
 * @param bpmMeanMin BPM promedio del minuto
 * @param vMeanMin Velocidad promedio del minuto (km/h)
 */
void guardarDatosCSV(float bpmMeanMin, float vMeanMin);

/**
 * @brief Reinicia todos los acumuladores del minuto actual
 */
void resetMinuteAccumulators();

#endif // SD_CARD_H
