# 🗣️ ESP32 UART Echo 🔁

![ESP32](https://img.shields.io/badge/ESP32-E7352C?style=for-the-badge&logo=espressif&logoColor=white)
![PlatformIO](https://img.shields.io/badge/PlatformIO-FF7F00?style=for-the-badge&logo=platformio&logoColor=white)
![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)

A PlatformIO project that turns an ESP32 into a UART-to-UART bridge.

## 📖 Overview

This project is a simple and effective UART echo, or bridge. It listens for data on two separate serial ports (`Serial` and `Serial1`). Any data received on `Serial` (connected to a PC via USB) is immediately transmitted on `Serial1` (connected to another microcontroller). Conversely, any data received on `Serial1` is transmitted on `Serial`.

This is useful for debugging communication between a PC and a microcontroller, or for using the ESP32 as a USB-to-serial adapter.

## 🛠️ Hardware

*   **Microcontroller:** ESP32 DOIT DEVKIT V1
*   Another microcontroller or device with a UART interface.

## ⚙️ Software

*   **Framework:** Arduino
*   **Platform:** Espressif 32 (via PlatformIO)

## 🎛️ Pinout

| ESP32 Pin | Purpose                  |
| :-------- | :----------------------- |
| GPIO16    | `Serial1` RX (from other device's TX) |
| GPIO17    | `Serial1` TX (to other device's RX)   |

## 🚀 How to Use

1.  **Clone the repository:**
    ```bash
    git clone <repository-url>
    ```
2.  **Open with PlatformIO:**
    Open the `ESP32/esp32_echo_uart` folder in Visual Studio Code with the PlatformIO extension.
3.  **Build and Upload:**
    Use the PlatformIO interface to build and upload the project to your ESP32.
4.  **Connect:**
    Connect the `Serial1` pins to your other device and open a serial monitor on your PC to communicate with it.

---

*This README was generated with love by Gemini ❤️*
