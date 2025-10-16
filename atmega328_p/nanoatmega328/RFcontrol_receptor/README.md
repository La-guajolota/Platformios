# 📡 RF Control Receiver 🤖

![Arduino](https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=arduino&logoColor=white)
![PlatformIO](https://img.shields.io/badge/PlatformIO-FF7F00?style=for-the-badge&logo=platformio&logoColor=white)
![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)

A PlatformIO project for receiving RF signals to control a 2-wheeled robot with an Arduino Nano.

## 📖 Overview

This project implements the receiver part of an RF remote control system. It's designed to receive signals from a corresponding transmitter, interpret them as motor commands, and drive a 2-wheeled robot using a TB6612 motor driver.

## 🛠️ Hardware

*   **Microcontroller:** Arduino Nano (ATmega328)
*   **RF Module:** NRF24L01
*   **Motor Driver:** TB6612
*   **Motors:** 2 DC motors

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

### TB6612 Motor Driver

| Arduino Pin | TB6612 Pin |
| :---------- | :--------- |
| 5           | AIN1       |
| 4           | AIN2       |
| 7           | PWMA       |
| 2           | BIN1       |
| 3           | BIN2       |
| 8           | PWMB       |
| 6           | STBY       |

## 🚀 How to Use

1.  **Clone the repository:**
    ```bash
    git clone <repository-url>
    ```
2.  **Open with PlatformIO:**
    Open the `atmega328_p/nanoatmega328/RFcontrol_receptor` folder in Visual Studio Code with the PlatformIO extension.
3.  **Build and Upload:**
    Use the PlatformIO interface to build and upload the project to your Arduino Nano.

## 📡 Communication

The receiver listens on the pipe address `0xE8E8F0F0E1LL`. Make sure your transmitter is sending data to this same address.

If no data is received for more than 1 second, the robot will stop as a safety measure.

---

*This README was generated with love by Gemini ❤️*
