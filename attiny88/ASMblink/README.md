# 💡 ASM Blink ✨

![AVR](https://img.shields.io/badge/AVR-Atmel-CC0000?style=for-the-badge&logo=atmel&logoColor=white)
![PlatformIO](https://img.shields.io/badge/PlatformIO-FF7F00?style=for-the-badge&logo=platformio&logoColor=white)
![Assembly](https://img.shields.io/badge/Assembly-6E4C13?style=for-the-badge&logo=assemblyscript&logoColor=white)

A PlatformIO project that blinks an LED in pure AVR assembly on an ATtiny88.

## 📖 Overview

This is the "Hello, World!" of embedded systems, written in AVR assembly. The code configures pin PD0 as an output and then enters an infinite loop that toggles the pin high and low, with a delay in between, to make an LED blink.

## 🛠️ Hardware

*   **Microcontroller:** ATtiny88
*   **LED:** A standard LED connected to pin PD0.

## ⚙️ Software

*   **Platform:** Atmel AVR (via PlatformIO)
*   **Language:** AVR Assembly

## 🚀 How to Use

1.  **Clone the repository:**
    ```bash
    git clone <repository-url>
    ```
2.  **Open with PlatformIO:**
    Open the `attiny88/ASMblink` folder in Visual Studio Code with the PlatformIO extension.
3.  **Build and Upload:**
    This project uses a custom upload command for a USBasp programmer. Make sure your `platformio.ini` is configured for your programmer.
    ```ini
    [env:attiny88]
    platform = atmelavr
    board = attiny88
    upload_protocol = custom
    upload_flags =
        -p
        t88
        -c
        usbasp
    upload_command = avrdude $UPLOAD_FLAGS -U flash:w:$SOURCE:i
    ```
    Then, use the PlatformIO interface to build and upload the project.

---

*This README was generated with love by Gemini ❤️*
