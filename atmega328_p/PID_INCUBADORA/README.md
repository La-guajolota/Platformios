# 🌡️ PID Incubator Controller 🐣

![Arduino](https://img.shields.io/badge/Arduino-00979D?style=for-the-badge&logo=arduino&logoColor=white)
![PlatformIO](https://img.shields.io/badge/PlatformIO-FF7F00?style=for-the-badge&logo=platformio&logoColor=white)
![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)

A PlatformIO project for controlling the temperature of an incubator using a PID controller on an Arduino Mega.

## 📖 Overview

This project implements a PID (Proportional-Integral-Derivative) control system to maintain a constant temperature in an incubator. It uses a DHT sensor to measure the temperature and controls a heating element via a Solid State Relay (SSR). The PID controller adjusts the power supplied to the heater to keep the temperature at a desired setpoint.

## 🛠️ Hardware

*   **Microcontroller:** Arduino Mega 2560
*   **Sensor:** DHT11 or DHT22 Temperature and Humidity Sensor
*   **Actuator:** Solid State Relay (SSR) connected to a heating element
*   **Zero-Cross Detector:** A circuit to detect the zero-crossing point of the AC waveform.

## ⚙️ Software

*   **Framework:** Arduino
*   **Platform:** Atmel AVR (via PlatformIO)
*   **Libraries:**
    *   `paulstoffregen/TimerOne@^1.2`
    *   `markruys/DHT@^1.0.0`

## 🎛️ Pinout

| Arduino Pin | Purpose                  |
| :---------- | :----------------------- |
| 3           | Zero-Cross Detector (ZC) |
| 7           | SSR Trigger (FIRE)       |
| 6           | DHT Sensor Data          |

## 🚀 How to Use

1.  **Clone the repository:**
    ```bash
    git clone <repository-url>
    ```
2.  **Open with PlatformIO:**
    Open the `atmega328_p/PID_INCUBADORA` folder in Visual Studio Code with the PlatformIO extension.
3.  **Build and Upload:**
    Use the PlatformIO interface to build and upload the project to your Arduino Mega.

## 💬 Serial Commands

You can tune the PID controller and change the setpoint in real-time using the serial monitor. The following commands are available:

*   `P<value>`: Set the proportional gain (Kp).
*   `I<value>`: Set the integral gain (Ki).
*   `D<value>`: Set the derivative gain (Kd).
*   `S<value>`: Set the desired temperature (setpoint).

For example, to set the setpoint to 37.5°C, send `S37.5` through the serial monitor.

## 📈 Serial Output

The Arduino sends the current temperature and the setpoint in CSV format, which can be easily plotted using the Arduino IDE's serial plotter.

`temperature,setpoint`

---

*This README was generated with love by Gemini ❤️*
