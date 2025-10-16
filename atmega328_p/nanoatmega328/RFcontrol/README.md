# 📡 RF Control Transmitter 🕹️

![Arduino](https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=arduino&logoColor=white)
![PlatformIO](https://img.shields.io/badge/PlatformIO-FF7F00?style=for-the-badge&logo=platformio&logoColor=white)
![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)

A PlatformIO project for sending RF signals from a joystick with an Arduino Nano.

## 📖 Overview

This project implements the transmitter part of an RF remote control system. It reads the X and Y axes of a joystick and sends the values wirelessly using an NRF24L01 module. This is the counterpart to the `RFcontrol_receptor` project.

## 🛠️ Hardware

*   **Microcontroller:** Arduino Nano (ATmega328)
*   **RF Module:** NRF24L01
*   **Joystick:** A standard 2-axis analog joystick.

## ⚙️ Software

*   **Framework:** Arduino
*   **Platform:** Atmel AVR (via PlatformIO)
*   **Libraries:**
    *   `nrf24/RF24@^1.4.11`

## 🎛️ Pinout

### NRF24L01

| Arduino Pin | NRF24L01 Pin |
| :---------- | :----------- |
| 9           | CE           |
| 10          | CSN          |
| 11          | MOSI         |
| 12          | MISO         |
| 13          | SCK          |

### Joystick

| Arduino Pin | Joystick Pin |
| :---------- | :----------- |
| A0          | VRX          |
| A1          | VRY          |

## 🚀 How to Use

1.  **Clone the repository:**
    ```bash
    git clone <repository-url>
    ```
2.  **Open with PlatformIO:**
    Open the `atmega328_p/nanoatmega328/RFcontrol` folder in Visual Studio Code with the PlatformIO extension.
3.  **Build and Upload:**
    Use the PlatformIO interface to build and upload the project to your Arduino Nano.

## 📡 Communication

The transmitter sends data to the pipe address `0xE8E8F0F0E1LL`. Make sure your receiver is listening on this same address.

## 🔬 Debug Mode

To enable debug mode, uncomment the following line in `src/main.cpp`:

```cpp
#define DEBUG_MODE
```

When enabled, the joystick values will be printed to the serial monitor.

---

*This README was generated with love by Gemini ❤️*
