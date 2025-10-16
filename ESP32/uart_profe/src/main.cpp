#include <Arduino.h>

unsigned long lastUART = 0;
const unsigned long intervalUART = 1000; // 1 second
 
void setup() {
  Serial.begin(115200);                     // UART0 (connected to USB by default)
  Serial2.begin(9600, SERIAL_8N1, 16, 17);  // UART2: RX=GPIO16, TX=GPIO17
}
 
void loop() {
  unsigned long currentMillis = millis();
 
  if (currentMillis - lastUART >= intervalUART) {
    lastUART = currentMillis;
    Serial.println("Message from UART0");
    Serial2.println("Message from UART2");
  }
}