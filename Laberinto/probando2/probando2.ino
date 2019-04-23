#define SHARPI A0
#define SHARPA 10
#define SHARPO A1

#define AV 90


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
  attachInterrupt(0, EncoderEventMotI_ROTO, CHANGE);
  attachInterrupt(1, EncoderEventMotD, CHANGE);
}

void loop() {
  dist_i = 2076.0 / (analogRead(SHARPI) - 11.0);
  dist_d = 2076.0 / (analogRead(SHARPO) - 11.0);
  if ((dist_d < 15)&&(dist_i < 15)){
    if (!digitalRead(SHARPA)){ //si no tenemos el PID y los giros muy bien se va a dar la vuelta todo el rato
      avanza_mm(25);
      if ((dist_d < 15)&&(dist_i < 15)){ //me evito que en esquinas me de la vuelta (explicar)
        para();
        delay(100);
        girar90D();
        girar90D();
        para();
        delay(500);
        }
      } else {
        avanzar_sharps(); //si tengo dos paredes voy con PID de paredes, sin dar mm
        }
    } else { //me parece más sencillo que solo vaya recto si tengo las dos paredes, es decir primero giro a derecha, luego a izquierda y luego recto)
      if (dist_d > 15){
        para();
        delay(2000);
        avanza_mm(AV);
        para();
        delay(100);
        girar90D();
        para();
        do {
          avanzar_encoders();
          } while (!((dist_d < 15)&&(dist_i < 15))); //me preocupa porque no se cumple cuando llego a la llegada, nunca se encuentra con otra pared
      }
      if (dist_i > 15){
        para();
        delay(2000);
        avanza_mm(AV);
        para();
        delay(100);
        girar90I();
        para();
        do {
          avanzar_encoders();
          } while (!((dist_d < 15)&&(dist_i < 15)));
          }
       }
}
