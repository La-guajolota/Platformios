#  IGBT Inverter Controller 💡

![Arduino](https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=arduino&logoColor=white)
![PlatformIO](https://img.shields.io/badge/PlatformIO-FF7F00?style=for-the-badge&logo=platformio&logoColor=white)
![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)

A PlatformIO project for controlling a three-phase IGBT inverter with an Arduino Uno.

## 📖 Overview

This project provides a simple and effective way to control six IGBTs for a three-phase inverter. It uses a circular shift register to generate the required switching sequence, converting a DC input into a three-phase AC output.

## 🛠️ Hardware

*   **Microcontroller:** ATmega328P (Arduino Uno)
*   **Components:** 6 IGBTs with their respective drivers and a DC power source.

## ⚙️ Software

*   **Framework:** Arduino
*   **Platform:** Atmel AVR (via PlatformIO)

## 🎛️ Pinout

The IGBTs are connected to Port B of the Arduino Uno:

| Arduino Pin | Port Pin | IGBT      | Phase |
| :---------- | :------- | :-------- | :---- |
| 8           | PB0      | IGBT 1    | A     |
| 9           | PB1      | IGBT 3    | B     |
| 10          | PB2      | IGBT 5    | C     |
| 11          | PB3      | IGBT 4    | A     |
| 12          | PB4      | IGBT 6    | B     |
| 13          | PB5      | IGBT 2    | C     |

## 🚀 How to Use

1.  **Clone the repository:**
    ```bash
    git clone <repository-url>
    ```
2.  **Open with PlatformIO:**
    Open the `atmega328_p/IGBT_inverter` folder in Visual Studio Code with the PlatformIO extension.
3.  **Build and Upload:**
    Use the PlatformIO interface to build and upload the project to your Arduino Uno.

## 🔬 Debug Mode

To enable debug mode, uncomment the following line in `src/main.cpp`:

```cpp
// #define DEBUG_MODE
```

When enabled, the current state of the control register will be printed to the serial monitor.

## ⚠️ Important Notes

*   This code does not include dead-time generation. It is crucial to implement a dead-time circuit in hardware to prevent short circuits between the high-side and low-side IGBTs of the same phase.
*   Ensure proper isolation between the microcontroller and the high-power IGBT circuits using optocouplers or dedicated IGBT drivers.
*   The switching frequency can be adjusted by changing the `IGBT_PERIOD_US` macro in `src/main.cpp`.

---

*This README was generated with love by Gemini ❤️*
