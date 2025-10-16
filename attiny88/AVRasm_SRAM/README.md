# 💾 AVR Assembly SRAM Example 💾

![AVR](https://img.shields.io/badge/AVR-Atmel-CC0000?style=for-the-badge&logo=atmel&logoColor=white)
![PlatformIO](https://img.shields.io/badge/PlatformIO-FF7F00?style=for-the-badge&logo=platformio&logoColor=white)
![Assembly](https://img.shields.io/badge/Assembly-6E4C13?style=for-the-badge&logo=assemblyscript&logoColor=white)

A PlatformIO project demonstrating how to access SRAM in pure AVR assembly on an ATtiny88.

## 📖 Overview

This project shows the basic operations of writing to and reading from SRAM (Static Random-Access Memory) in AVR assembly. The code loads a value into a register and then stores it at a specific address in SRAM. It then shows (in a commented-out line) how to load that value back into a register.

Understanding SRAM is fundamental for storing and manipulating data in your programs.

## 🛠️ Hardware

*   **Microcontroller:** ATtiny88

## ⚙️ Software

*   **Platform:** Atmel AVR (via PlatformIO)
*   **Language:** AVR Assembly

## 🚀 How to Use

1.  **Clone the repository:**
    ```bash
    git clone <repository-url>
    ```
2.  **Open with PlatformIO:**
    Open the `attiny88/AVRasm_SRAM` folder in Visual Studio Code with the PlatformIO extension.
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
