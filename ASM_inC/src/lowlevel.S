; codigo_ensamblador.S
.global _start  ; Hacer visible la etiqueta _start para que sea utilizada por el enlazador

_start:
    ldi r16, 0x01       ; Cargar valor 0x01 en el registro r16
    out 0x0B, r16      ; Establecer el bit 0 de PORTD (enciende el LED)
    
delay_loop1:
    ldi r18, 0xFF       ; Cargar valor 0xFF en r18 para el retraso
delay_loop1_body:
    dec r18             ; Decrementar r18
    brne delay_loop1_body ; Saltar si r18 no es cero
    
    ldi r16, 0x00       ; Limpiar el bit 0 de PORTD (apaga el LED)
    out 0x0B, r16      ; Enviar valor a PORTD

delay_loop2:
    ldi r18, 0xFF       ; Cargar valor 0xFF en r18 para otro retraso
delay_loop2_body:
    dec r18             ; Decrementar r18
    brne delay_loop2_body ; Saltar si r18 no es cero
    rjmp _start         ; Volver al inicio
