#define __SFR_OFFSET 0
#include <avr/io.h>
#include <avr/common.h>

#define SRAM_START 0x0100

.global main

main:
  ldi r16, 0xAA         ;Data to be stored to SRAM
  sts SRAM_START, r16   ;Store data from register to SRAM
  lds r1, SRAM_START    ; Load data from SRAM to register

  ldi 
loop:
  rjmp loop