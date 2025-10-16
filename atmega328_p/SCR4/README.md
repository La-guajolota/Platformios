# ⚡ SCR Firing Controller ⚡

![Arduino](https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=arduino&logoColor=white)
![PlatformIO](https://img.shields.io/badge/PlatformIO-FF7F00?style=for-the-badge&logo=platformio&logoColor=white)
![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)

A PlatformIO project for controlling the firing angle of four SCRs in a controlled rectifier configuration using an Arduino Uno.

## 📖 Overview

This project provides a precise, interrupt-based method for controlling the firing angle of Silicon Controlled Rectifiers (SCRs) in a full-wave controlled rectifier. It uses a zero-cross detection signal to synchronize the firing pulses, allowing for smooth control over the output voltage.

The firing angle is automatically swept from 0 to 180 degrees and back, demonstrating the control capabilities of the system.

## 🛠️ Hardware

*   **Microcontroller:** Arduino Uno (ATmega328P)
*   **SCRs:** 4 SCRs arranged in a full-bridge configuration.
*   **Zero-Cross Detector:** A circuit that provides a pulse every time the AC waveform crosses zero.

## ⚙️ Software

*   **Framework:** Arduino
*   **Platform:** Atmel AVR (via PlatformIO)

## 🎛️ Pinout

| Arduino Pin | Port Pin | Purpose                  |
| :---------- | :------- | :----------------------- |
| 8           | PB0      | Zero-Cross Input         |
| 9           | PB1      | SCR Pair 1 Firing Pulse  |
| 10          | PB2      | SCR Pair 2 Firing Pulse  |

## 🚀 How to Use

1.  **Clone the repository:**
    ```bash
    git clone <repository-url>
    ```
2.  **Open with PlatformIO:**
    Open the `atmega328_p/SCR4` folder in Visual Studio Code with the PlatformIO extension.
3.  **Build and Upload:**
    Use the PlatformIO interface to build and upload the project to your Arduino Uno.

## ⚠️ Important Notes

*   **Isolation is crucial!** Always use optocouplers or other isolation methods between the Arduino and the high-voltage SCR circuit to protect your microcontroller.
*   The `trigger_delay_us` variable controls the firing angle. You can modify the `loop()` function to set a fixed firing angle or to control it with a potentiometer or other input.

---

*This README was generated with love by Gemini ❤️*
