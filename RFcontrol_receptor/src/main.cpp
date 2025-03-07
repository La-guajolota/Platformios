#include <Arduino.h>
#include <SPI.h>
#include "RF24.h"
#include "nRF24L01.h"

// DEFINICIONES DE LOS PINES
/* SPI */
#define CE_PIN 9
#define CSN_PIN 10
/* Definición de pines para el driver TB6612 */
#define AIN1 5 
#define AIN2 4
#define PWMA 7
#define BIN1 2
#define BIN2 3
#define PWMB 8
#define STBY 6

// CONSTRUCTOR PARA EL MODULO DE RF
RF24 radio(CE_PIN, CSN_PIN);

// STRUCT
struct Received_data {
  byte ch1;
  byte ch2;
} received_data;

// VARIABLES GLOBALES
const uint64_t my_radio_pipe = 0xE8E8F0F0E1LL;
unsigned long lastRecvTime = 0;
const int max_pwm = 255;
const int min_pwm = -255;
const int max_ctl_rf = 2000;
const int min_ctl_rf = 950;
int forwardBackward; // Almacena dirección y la velocidad Foward-backawrds
int leftRight;       // Almacena dirección y velocidad Left-right
int speed1;           
int speed2;

// DEFINICIÓN DE FUNCIONES 
void receive_the_data();
void setMotor(int pwmPin, int in1, int in2, int speed);
void stopIfFault();
void reset_the_Data();

void setup() {
  // COMUNICACIÓN
  Serial.begin(9600); // Inicializa la comunicación serie
  
  // GPIO CONFIGURACIONES
  /* Configuración de los pines del driver como salidas */
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(STBY, OUTPUT);

  digitalWrite(STBY, HIGH); // Habilita el driver TB6612

  // Inicializamos el módulo RF
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.openWritingPipe(my_radio_pipe);

  radio.startListening();
}

void loop() {
  receive_the_data();

  //Checamos si seperido la señal
  unsigned long now = millis();
  if ( now - lastRecvTime > 1000 ) {
    reset_the_Data();
  } 

  // Convertir los valores de los joystics a un rango utilizable para los motores
  forwardBackward = map(received_data.ch1, min_ctl_rf, max_ctl_rf, min_pwm, max_ctl_rf); // Mapeo de valores para velocidad
  leftRight = map(received_data.ch2, min_ctl_rf, max_ctl_rf, min_pwm, max_ctl_rf);       // Mapeo de valores para giro

  // Ajustar velocidades combinando avance/retroceso con giro
  speed1 = forwardBackward + leftRight;
  speed2 = forwardBackward - leftRight;

  // Limitar los valores a los rangos válidos
  speed1 = constrain(speed1, min_pwm, max_ctl_rf);
  speed2 = constrain(speed2, min_pwm, max_ctl_rf);

  // Aplicar velocidades a los motores
  setMotor(PWMA, AIN1, AIN2, speed1);
  setMotor(PWMB, BIN1, BIN2, speed2);
} 

//We create the function that will read the data each certain time
void receive_the_data()
{
  while ( radio.available() ) {
    radio.read(&received_data, sizeof(Received_data));
    lastRecvTime = millis(); //Here we receive the data
  }
}

// 
void stopIfFault()
{
  Serial.println("Checking for faults (not available in TB6612)"); // Mensaje de depuración, sin función real en TB6612
}

// Función para controlar la velocidad y dirección de un motor
void setMotor(int pwmPin, int in1, int in2, int speed)
{
  if (speed > 0) // Movimiento hacia adelante
  {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  }
  else if (speed < 0) // Movimiento hacia atrás
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  }
  else // Detener el motor
  {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
  }
  analogWrite(pwmPin, abs(speed)); // Ajusta la velocidad del motor
}

//
void reset_the_Data() 
{
  received_data.ch1 = 0;      
  received_data.ch2 = 127;
}