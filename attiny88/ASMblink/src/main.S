#define __SFR_OFFSET 0
#include <avr/io.h>
#include <avr/common.h>

; Author: Adrian Silva Palafox
; Date: 2024
; Description: This assembly code blinks an LED connected to pin PD0.

.global main

main:
  sbi   DDRD, 0  ; Set PD0 as an output

blink:
  sbi    PORTD, 0 ; Set PD0 high (turn LED on)
  rcall  delay
  cbi    PORTD, 0 ; Clear PD0 low (turn LED off)
  rcall  delay
  rjmp   blink

; Delay subroutine
delay:
  ldi   r16, 0xFF ; Load outer loop counter
delloop1:
  ldi   r17, 0xFF ; Load inner loop counter
delloop2:
  dec   r17       ; Decrement inner loop counter
  brne  delloop2  ; Branch if not zero
  dec   r16       ; Decrement outer loop counter
  brne  delloop1  ; Branch if not zero
  ret             ; Return from subroutine
