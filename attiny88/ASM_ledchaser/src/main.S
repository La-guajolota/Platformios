#define __SFR_OFFSET 0
#include "avr/io.h"
#include "avr/common.h"

.def s_value = r18          ; Shift value register - stores value to shift
.def dir = r19              ; Direction register - direction to shift
.equ s_left = 0             ; Left shift flag
.equ s_right = 1            ; Right shift flag

.global main

main:
; Initialize the pins and registers
ldi r16, 0x1F              ; Pins PB0 to PB4 as output pins
out DDRB, r16              ; Set DDRB register (Data Direction Register) for output
ldi s_value, 0x01          ; Load shift register with initial value
ldi dir, s_left            ; Flag initial shift direction as left
loop:
    out PORTB, s_value     ; Display shift value on LEDs (PORTB output)
    rcall delay            ; Call delay subroutine

    cpi dir, s_left        ; Compare direction with left shift flag
    brne shift_right       ; If direction is not left, jump to shift_right

    lsl s_value            ; Rotate the register left
    sbrs s_value, 5        ; Skip next instruction if bit 5 is set (overflow)
    rjmp loop              ; Jump back to loop

    lsr s_value            ; Shift the set bit to bit 4
    ldi dir, s_right       ; Change shift direction to right

shift_right:
    lsr s_value            ; Rotate the register right
    sbrc s_value, 0        ; Skip next instruction if bit 0 is clear
    ldi dir, s_left        ; Change shift direction to left
    rjmp loop              ; Jump back to loop

; Delay subroutine
delay:
    ldi r16, 0xff          ; Load delay counter
delloop1:
    ldi r17, 0xff          ; Load inner loop counter
delloop2:
    dec r17                ; Decrement inner loop counter
    brbc SREG_Z, delloop2  ; Repeat inner loop until zero
    dec r16                ; Decrement outer loop counter
    brbc SREG_Z, delloop1  ; Repeat outer loop until zero
    ret                     ; Return from subroutine
