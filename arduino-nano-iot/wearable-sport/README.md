# Sistema de Monitoreo Deportivo - Estructura Modular

## Estructura del Proyecto

```
proyecto/
├── src/
│   ├── main.cpp                  # Archivo principal con setup() y loop()
│   ├── config.h                  # Configuraciones y constantes
│   ├── filter.h                  # Interfaz de filtros biquad
│   ├── filter.cpp                # Implementación de filtros
│   ├── heart_rate.h              # Interfaz de detección de FC
│   ├── heart_rate.cpp            # Implementación de detección de FC
│   ├── gps_processing.h          # Interfaz de procesamiento GPS
│   ├── gps_processing.cpp        # Implementación de GPS
│   ├── velocity_zones.h          # Interfaz de zonas de velocidad
│   ├── velocity_zones.cpp        # Implementación de zonas de velocidad
│   ├── heart_rate_zones.h        # Interfaz de zonas de FC
│   ├── heart_rate_zones.cpp      # Implementación de zonas de FC
│   ├── metrics.h                 # Interfaz de métricas de entrenamiento
│   ├── metrics.cpp               # Implementación de métricas
│   ├── sd_card.h                 # Interfaz de funciones SD
│   └── sd_card.cpp               # Implementación de funciones SD
├── lib/                          # Bibliotecas de PlatformIO
└── platformio.ini                # Configuración de PlatformIO
```

## Descripción de Módulos

### 1. **config.h**
- Define todas las constantes de configuración del sistema
- Pines de hardware (ADC, SD, etc.)
- Parámetros de muestreo y comunicación
- Umbrales y constantes de zonas

### 2. **filter.h/cpp**
- Estructura `Biquad` para filtros IIR de segundo orden
- Banco de 4 filtros en cascada para ECG
- Función `filterSample()` para procesamiento de señal

### 3. **heart_rate.h/cpp**
- Variables para detección de latidos cardíacos
- Buffer circular de intervalos RR
- Función `pushRR()` para cálculo de BPM promedio
- Algoritmo adaptativo de detección de picos

### 4. **gps_processing.h/cpp**
- Objeto `TinyGPSPlus` para parseo de NMEA
- Promedio móvil de velocidad (`movingAvg()`)
- Conversión de hora UTC a local (`wrapLocalHour()`)

### 5. **velocity_zones.h/cpp**
- Enumeración de zonas de velocidad (CAM/TRO/CAR/SPR)
- Acumuladores de tiempo y distancia por zona
- Función `velZoneIndex()` para clasificación

### 6. **heart_rate_zones.h/cpp**
- Enumeración de zonas de FC (HZ1-HZ6)
- Acumuladores de tiempo por zona
- Función `hrZoneIndex()` basada en % de FC máxima

### 7. **metrics.h/cpp**
- Variables de métricas globales (distancia, TRIMP)
- Detección de sprints con histéresis
- Temporizadores del sistema

### 8. **sd_card.h/cpp**
- Creación de archivo CSV con encabezados
- Guardado periódico de datos cada minuto
- Formateo de timestamp con GPS
- Reseteo de acumuladores

### 9. **main.cpp**
- Función `setup()`: inicialización del sistema
- Función `loop()`: bucle principal con tres tareas:
  - Muestreo ECG a 30 Hz
  - Procesamiento GPS cada segundo
  - Resumen y guardado cada minuto

## Flujo de Datos

```
ECG Signal (30 Hz)
    ↓
Filter Module → Heart Rate Detection
                      ↓
                   BPM value
                      ↓
                Heart Rate Zones
                      ↓
GPS Data (1 Hz)      Metrics & TRIMP
    ↓                    ↓
GPS Processing           ↓
    ↓                    ↓
Velocity Zones    ← ← ← ←
    ↓
Sprint Detection
    ↓
    ↓← ← ← ← ← ← ← ← ← ← ← 
    ↓                      
SD Card Module (every 60s)
```

## Dependencias

- **Arduino.h**: Funciones básicas de Arduino
- **TinyGPSPlus**: Parseo de datos GPS
- **SD.h**: Lectura/escritura en tarjeta SD
- **SPI.h**: Comunicación con SD

## Notas de Compilación

Este proyecto está diseñado para **PlatformIO**. Asegúrate de:

1. Tener instaladas las bibliotecas necesarias en `lib/` o `lib_deps`
2. Configurar correctamente `platformio.ini` con tu placa objetivo
3. Todos los archivos `.h` y `.cpp` deben estar en el directorio `src/`

## Ventajas de la Modularización

✅ **Mantenibilidad**: Cada módulo tiene una responsabilidad clara  
✅ **Reutilización**: Los módulos pueden usarse en otros proyectos  
✅ **Testing**: Facilita pruebas unitarias de componentes individuales  
✅ **Legibilidad**: Código organizado y fácil de navegar  
✅ **Colaboración**: Múltiples desarrolladores pueden trabajar en paralelo  
✅ **Compilación**: Compilación incremental más rápida