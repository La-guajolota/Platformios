#define __SFR_OFFSET 0
#include <avr/io.h>
#include <avr/common.h>

; Author: Adrian Silva Palafox
; Date: 2024
; Description: This assembly code demonstrates how to store and load data from SRAM.

#define SRAM_START 0x0100 ; The start address of SRAM on the ATtiny88

.global main

main:
  ldi r16, 0xAA         ; Load the data to be stored into r16
  sts SRAM_START, r16   ; Store the data from the register to the specified SRAM address
  
  ; The next line is commented out because it would overwrite the previous value
  ; lds r1, SRAM_START    ; Load the data from the SRAM address into r1

loop:
  rjmp loop             ; Infinite loop
