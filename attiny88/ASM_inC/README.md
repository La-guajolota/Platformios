# 🧑‍💻 Assembly in C 💡

![AVR](https://img.shields.io/badge/AVR-Atmel-CC0000?style=for-the-badge&logo=atmel&logoColor=white)
![PlatformIO](https://img.shields.io/badge/PlatformIO-FF7F00?style=for-the-badge&logo=platformio&logoColor=white)
![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Assembly](https://img.shields.io/badge/Assembly-6E4C13?style=for-the-badge&logo=assemblyscript&logoColor=white)

A PlatformIO project demonstrating how to call assembly code from a C program on an ATtiny88.

## 📖 Overview

This project is a simple example of how to integrate AVR assembly code with a C program. The C code sets up the direction of a pin, and then calls an assembly routine in a loop. The assembly routine is responsible for blinking an LED connected to that pin.

This is a common technique for performance-critical tasks or when direct hardware manipulation is required.

## 🛠️ Hardware

*   **Microcontroller:** ATtiny88
*   **LED:** A standard LED connected to pin PD0.

## ⚙️ Software

*   **Platform:** Atmel AVR (via PlatformIO)
*   **Languages:** C and AVR Assembly

## 🚀 How to Use

1.  **Clone the repository:**
    ```bash
    git clone <repository-url>
    ```
2.  **Open with PlatformIO:**
    Open the `attiny88/ASM_inC` folder in Visual Studio Code with the PlatformIO extension.
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
