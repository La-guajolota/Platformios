#define __SFR_OFFSET 0
#include "avr/io.h"
#include "avr/common.h"

; Author: Adrian Silva Palafox
; Date: 2024
; Description: This assembly code demonstrates how to initialize and use the stack.

; RAMEND for ATtiny88 is 0x2FF
#define low_RAMEND  0xFF
#define high_RAMEND 0x02

.global main   ; Define "main" as the entry point

main:   
    ; Initialize the stack pointer
    ldi r16, low_RAMEND    ; Load the low byte of RAMEND (0xFF)
    out spl, r16           ; Set the low part of the stack pointer
    ldi r16, high_RAMEND   ; Load the high byte of RAMEND (0x02)
    out sph, r16           ; Set the high part of the stack pointer

    ; Push and pop a value to/from the stack
    ldi r16, 0x5A          ; Load the value 0x5A into r16
    push r16               ; Push r16 onto the stack
    clr r16                ; Clear r16
    pop r16                ; Pop the value from the stack back into r16

loop:
    rjmp loop              ; Infinite loop