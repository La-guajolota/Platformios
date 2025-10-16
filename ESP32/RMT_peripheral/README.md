# 📡 ESP32 RMT Peripheral Example 펄스

![ESP32](https://img.shields.io/badge/ESP32-E7352C?style=for-the-badge&logo=espressif&logoColor=white)
![PlatformIO](https://img.shields.io/badge/PlatformIO-FF7F00?style=for-the-badge&logo=platformio&logoColor=white)
![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)

A PlatformIO project demonstrating the use of the ESP32's RMT (Remote Control) peripheral to generate precise pulse trains.

## 📖 Overview

This project showcases the capabilities of the ESP32's RMT peripheral for generating complex and accurately timed pulse sequences. The RMT peripheral is designed for infrared remote control protocols, but it's versatile enough for many other applications, such as driving stepper motors, controlling addressable LEDs, or generating custom digital waveforms.

This example configures three RMT channels to output pulse trains on three different GPIO pins.

## 🛠️ Hardware

*   **Microcontroller:** ESP32 DOIT DEVKIT V1
*   An oscilloscope or logic analyzer to view the output signals (optional but recommended).

## ⚙️ Software

*   **Framework:** Arduino
*   **Platform:** Espressif 32 (via PlatformIO)

## 🎛️ Pinout

| ESP32 Pin | RMT Channel |
| :-------- | :---------- |
| GPIO27    | Channel 0   |
| GPIO26    | Channel 1   |
| GPIO25    | Channel 2   |

## 🚀 How to Use

1.  **Clone the repository:**
    ```bash
    git clone <repository-url>
    ```
2.  **Open with PlatformIO:**
    Open the `ESP32/RMT_peripheral` folder in Visual Studio Code with the PlatformIO extension.
3.  **Build and Upload:**
    Use the PlatformIO interface to build and upload the project to your ESP32.
4.  **Observe:**
    Connect an oscilloscope or logic analyzer to the output pins to see the generated waveforms.

---

*This README was generated with love by Gemini ❤️*
