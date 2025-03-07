#include <Arduino.h>

void setup() {
  // Inicializa Serial para comunicación con la PC
  Serial.begin(115200);
  
  // Inicializa Serial1 para comunicación con el MCU externo
  Serial1.begin(115200, SERIAL_8N1, 16, 17); // RX = GPIO16, TX = GPIO17
}

void loop() {
  // Lee datos del MCU externo y los envía a la PC
  if (Serial1.available()) {
    int data = Serial1.read();
    Serial.write(data);
  }

  // Lee datos de la PC y los envía al MCU externo
  if (Serial.available()) {
    int data = Serial.read();
    Serial1.write(data);
  }
}
