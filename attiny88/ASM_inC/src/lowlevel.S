; lowlevel.S
; Author: Adrian Silva Palafox
; Date: 2024
; Description: This assembly code toggles a LED connected to PORTD, pin 0.

.global _start  ; Make the _start label visible to the linker

_start:
    ; Turn LED on
    ldi r16, 0x01       ; Load the value 0x01 into register r16
    out 0x0B, r16       ; Set bit 0 of PORTD (turn on the LED)
    
    ; Delay loop
delay_loop1:
    ldi r18, 0xFF       ; Load 0xFF into r18 for the delay
delay_loop1_body:
    dec r18             ; Decrement r18
    brne delay_loop1_body ; Branch if r18 is not zero
    
    ; Turn LED off
    ldi r16, 0x00       ; Clear bit 0 of PORTD (turn off the LED)
    out 0x0B, r16       ; Write the value to PORTD

delay_loop2:
    ldi r18, 0xFF       ; Load 0xFF into r18 for another delay
delay_loop2_body:
    dec r18             ; Decrement r18
    brne delay_loop2_body ; Branch if r18 is not zero
    
    rjmp _start         ; Jump back to the beginning