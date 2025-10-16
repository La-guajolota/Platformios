#define __SFR_OFFSET 0
#include "avr/io.h"
#include "avr/common.h"

; Author: Adrian Silva Palafox
; Date: 2024
; Description: This assembly code creates a Larson scanner (LED chaser) effect on 5 LEDs connected to PORTB.

.def s_value = r18          ; Shift value register - stores the value to be shifted
.def dir = r19              ; Direction register - stores the direction of the shift
.equ s_left = 0             ; Flag for left shift
.equ s_right = 1            ; Flag for right shift

.global main

main:
    ; Initialize the pins and registers
    ldi r16, 0x1F              ; Set pins PB0 to PB4 as output pins
    out DDRB, r16              ; Write to the Data Direction Register for PORTB
    ldi s_value, 0x01          ; Load the initial value for the shift register (LED 1 on)
    ldi dir, s_left            ; Set the initial shift direction to left

loop:
    out PORTB, s_value     ; Output the shift value to the LEDs on PORTB
    rcall delay            ; Call the delay subroutine

    cpi dir, s_left        ; Compare the direction with the left shift flag
    brne shift_right       ; If the direction is not left, jump to shift_right

    ; Shift left
    lsl s_value            ; Logical shift left of the s_value register
    sbrs s_value, 5        ; Skip the next instruction if bit 5 is set (overflow)
    rjmp loop              ; If no overflow, jump back to the loop

    ; Change direction to right
    lsr s_value            ; Logical shift right to bring the bit back to position 4
    ldi dir, s_right       ; Change the shift direction to right
    rjmp loop

shift_right:
    lsr s_value            ; Logical shift right of the s_value register
    brbs s_value, 0        ; Branch if bit 0 is set (reached the end)
    rjmp loop              ; If not at the end, continue shifting right

    ; Change direction to left
    ldi dir, s_left        ; Change the shift direction to left
    rjmp loop              ; Jump back to the loop

; Delay subroutine
delay:
    ldi r16, 0xff          ; Load the outer loop counter
delloop1:
    ldi r17, 0xff          ; Load the inner loop counter
delloop2:
    dec r17                ; Decrement the inner loop counter
    brne delloop2          ; Repeat the inner loop until it reaches zero
    dec r16                ; Decrement the outer loop counter
    brne delloop1          ; Repeat the outer loop until it reaches zero
    ret                    ; Return from the subroutine