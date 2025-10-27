#include <Arduino.h>

#define led 8
#define vibrate 9

void setup()
{
  // leds
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  // vibrate motor
  pinMode(vibrate, OUTPUT);
  digitalWrite(vibrate, LOW);
}

void loop()
{
  // turn on leds and vibrate motor for 1 second
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(led, HIGH);
  digitalWrite(vibrate, HIGH);
  delay(1000);

  // turn off leds and vibrate motor for 1 second
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(led, LOW);
  digitalWrite(vibrate, LOW);
  delay(1000);
}
