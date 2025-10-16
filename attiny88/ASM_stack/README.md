# 📚 ASM Stack Example 🥞

![AVR](https://img.shields.io/badge/AVR-Atmel-CC0000?style=for-the-badge&logo=atmel&logoColor=white)
![PlatformIO](https://img.shields.io/badge/PlatformIO-FF7F00?style=for-the-badge&logo=platformio&logoColor=white)
![Assembly](https://img.shields.io/badge/Assembly-6E4C13?style=for-the-badge&logo=assemblyscript&logoColor=white)

A PlatformIO project that demonstrates stack initialization and usage in pure AVR assembly on an ATtiny88.

## 📖 Overview

This project provides a fundamental example of how to work with the stack in AVR assembly. The code first initializes the stack pointer to the end of RAM. Then, it pushes a value onto the stack, clears the register, and pops the value back from the stack to demonstrate that the value was successfully stored and retrieved.

This is a crucial concept for managing data, function calls, and interrupts in assembly programming.

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
    Open the `attiny88/ASM_stack` folder in Visual Studio Code with the PlatformIO extension.
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
