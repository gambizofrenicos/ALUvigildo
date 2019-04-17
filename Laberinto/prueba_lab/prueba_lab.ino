#define SHARPI A0
#define SHARPA 10
#define SHARPO A1


#include "encoder.h"
#include "pid.h"
#include "motores.h"

float dist_i;
float dist_d;

void setup() {
  // Inicializamos motor I
  pinMode(ENC1_MOTI, INPUT);
  pinMode(ENC2_MOTI, INPUT);
  pinMode(PWMI, OUTPUT);
  pinMode(DIRI, OUTPUT);

  // Inicializamos motor D
  pinMode(ENC1_MOTD, INPUT);
  pinMode(ENC2_MOTD, INPUT);
  pinMode(PWMD, OUTPUT);
  pinMode(DIRD, OUTPUT);
  // initialize hardware interrupts
  attachInterrupt(0, EncoderEventMotI, CHANGE);
  attachInterrupt(1, EncoderEventMotD, CHANGE);

  Serial.begin(9600);
}

void loop() {
  //girar90D_coche();
 // para();
  //delay(2000);
  dist_i = 2076.0 / (analogRead(SHARPI) - 11.0);
  dist_d = 2076.0 / (analogRead(SHARPO) - 11.0);
  digitalWrite(DIRI, HIGH);
  digitalWrite(DIRD, LOW);
   if (dist_d > 15){
    //Giro dcha
   // para();
   // delay(1000);
    avanza_mm_lab(40);
  //  para();
  //  delay(1000);
    girar90D();
   // para();
   // delay(1000);
    avanza_mm_lab(50);
  } else if (dist_i > 15){
    //Giro Izda
  //  para();
  //  delay(1000);
    avanza_mm_lab(40);
   // para();
   // delay(1000);
    girar90I();
   // para();
   // delay(1000);
    avanza_mm_lab(50);
  } else if (!digitalRead(SHARPA)){
    //Media vuelta
   // para();
   // delay(1000);
    girar90D();
    girar90D();
   // para();
   // delay(1000);
    avanza_mm_lab(50);
  } else {
    error(dist_i, dist_d);
    pwmi = PWM - PID_lab;
    pwmd = PWM + PID_lab;
    avanzar();
    }
}
