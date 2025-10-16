# 🌐 ESP32 Web Server Counter 📟

![ESP32](https://img.shields.io/badge/ESP32-E7352C?style=for-the-badge&logo=espressif&logoColor=white)
![PlatformIO](https://img.shields.io/badge/PlatformIO-FF7F00?style=for-the-badge&logo=platformio&logoColor=white)
![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)

A PlatformIO project that runs a simple web server on an ESP32 to display a counter.

## 📖 Overview

This project demonstrates the basics of creating a web server with an ESP32. The ESP32 connects to a WiFi network and starts a server on port 80. When a client connects to the ESP32's IP address, it serves a simple HTML page that displays a counter. The counter increments every 250 milliseconds, and the built-in LED blinks at the same rate.

## 🛠️ Hardware

*   **Microcontroller:** ESP32 Dev Module

## ⚙️ Software

*   **Framework:** Arduino
*   **Platform:** Espressif 32 (via PlatformIO)

## 🚀 How to Use

1.  **Clone the repository:**
    ```bash
    git clone <repository-url>
    ```
2.  **Create `server.h`:**
    In the `src` folder, create a file named `server.h` with the following content, replacing `<Your-SSID>` and `<Your-Password>` with your WiFi credentials:
    ```cpp
    const char* ssid = "<Your-SSID>";
    const char* password = "<Your-Password>";
    ```
3.  **Open with PlatformIO:**
    Open the `ESP32/esp01_test0` folder in Visual Studio Code with the PlatformIO extension.
4.  **Build and Upload:**
    Use the PlatformIO interface to build and upload the project to your ESP32.
5.  **Connect:**
    Open the serial monitor to find the IP address of your ESP32. Then, open a web browser and navigate to that IP address to see the counter.

---

*This README was generated with love by Gemini ❤️*
