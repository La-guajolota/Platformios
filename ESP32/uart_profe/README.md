# 📨 ESP32 UART Broadcaster 📡

![ESP32](https://img.shields.io/badge/ESP32-E7352C?style=for-the-badge&logo=espressif&logoColor=white)
![PlatformIO](https://img.shields.io/badge/PlatformIO-FF7F00?style=for-the-badge&logo=platformio&logoColor=white)
![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)

A PlatformIO project that sends messages over two UART ports on an ESP32.

## 📖 Overview

This simple project demonstrates how to use multiple UART (serial) ports on the ESP32. Every second, it sends a message to `Serial` (the default USB serial port) and `Serial2` (a hardware serial port on GPIO pins 16 and 17).

This is useful for understanding how to communicate with multiple serial devices simultaneously.

## 🛠️ Hardware

*   **Microcontroller:** ESP32 DOIT DEVKIT V1
*   A USB-to-serial converter or another microcontroller to listen to `Serial2`.

## ⚙️ Software

*   **Framework:** Arduino
*   **Platform:** Espressif 32 (via PlatformIO)

## 🎛️ Pinout

| ESP32 Pin | Purpose                  |
| :-------- | :----------------------- |
| GPIO16    | `Serial2` RX             |
| GPIO17    | `Serial2` TX             |

## 🚀 How to Use

1.  **Clone the repository:**
    ```bash
    git clone <repository-url>
    ```
2.  **Open with PlatformIO:**
    Open the `ESP32/uart_profe` folder in Visual Studio Code with the PlatformIO extension.
3.  **Build and Upload:**
    Use the PlatformIO interface to build and upload the project to your ESP32.
4.  **Observe:**
    Open the serial monitor to see the messages from `Serial`. Connect a serial-to-USB converter to pins 16 and 17 to see the messages from `Serial2`.

---

*This README was generated with love by Gemini ❤️*
