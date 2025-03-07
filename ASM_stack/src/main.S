#define __SFR_OFFSET 0
#include "avr/io.h"
#include "avr/common.h"

; RAMEND = 0x2FF
#define low_RAMEND  0xFF
#define high_RAMEND 0x02

.global main   ; Define "main" como el punto de entrada

main:   
    ldi r16, low_RAMEND    ; Cargar el byte bajo de RAMEND (0xFF)
    out spl, r16           ; Configurar la parte baja del puntero de pila
    ldi r16, high_RAMEND   ; Cargar el byte alto de RAMEND (0x02)
    out sph, r16           ; Configurar la parte alta del puntero de pila

    ldi r16, 0x5A          ; Cargar el valor 0x5A en r16
    push r16               ; Guardar r16 en la pila
    clr r16                ; Limpiar r16
    pop r16                ; Recuperar el valor de la pila en r16

loop:
    rjmp loop              ; Salto incondicional al inicio de "loop"
