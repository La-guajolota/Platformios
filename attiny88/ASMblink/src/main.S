#define __SFR_OFFSET 0
#include <avr/io.h>
#include <avr/common.h>

.global main

main:
  sbi   DDRD, DD0  ;Seteamos gpio como output

blink:
  sbi    PORTD, DD0 ;Set
  rcall  delay
  cbi    PORTD, DD0 ;Clear
  rcall  delay
  rjmp   blink

;Subrutina delay
delay:
  ldi   r16, 0xFF
delloop1:
  ldi   r17, 0xFF
delloop2:
  dec   r17                 ; Delay por decremento
  brbc  SREG_Z, delloop2    ;Branch si Status limpio
  dec   r16 
  brbc  SREG_Z, delloop1
  ret