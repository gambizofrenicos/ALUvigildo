#define SHARPI A0
#define SHARPA 10
#define SHARPO A1

#define AV 60

#define DIST 15

#include "encoder.h"
#include "pid.h"
#include "motores.h"


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

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  // initialize hardware interrupts
  attachInterrupt(0, EncoderEventMotI_ROTO, CHANGE);
  attachInterrupt(1, EncoderEventMotD, CHANGE);

  Serial.begin(9600);
}

void loop() {
  leer();
  if ((dist_d < DIST) && (dist_i < DIST)) {
    if (!dist_a) { //si no tenemos el PID y los giros muy bien se va a dar la vuelta todo el rato
      //Serial.println("oh, creo que hay pared delante");
      avanza_mm(25);
      leer();
      if ((dist_d < DIST) && (dist_i < DIST)) { //me evito que en esquinas me de la vuelta (explicar)
        //Serial.println("oh, SÍ que hay pared delante");
        para();
        delay(100);
        girar90D();
        girar90D();
        para();
        delay(500);
      }
    } else {
      //   Serial.println("TENGO PAREDES A LOS LADOS :D");
      avanzar_sharps(); //si tengo dos paredes voy con PID de paredes, sin dar mm
    }
  } else {
    leer();//me parece más sencillo que solo vaya recto si tengo las dos paredes, es decir primero giro a derecha, luego a izquierda y luego recto)
    if (dist_d > DIST) {
      digitalWrite(13, HIGH);
      para();
      delay(2000);
      avanza_mm(AV);
      para();
      delay(100);
      girar90D();
      para();
      delay(100);
      CountD = 0;
      CountI = 0;
    }
    else if (dist_i > DIST) {
      digitalWrite(13, LOW);
      para();
      delay(2000);
      avanza_mm(AV);
      para();
      delay(100);
      girar90I();
      para();
      delay(200);
      CountD = 0;
      CountI = 0;
    }
    para();
    delay(2000);
    avanza_mm(AV);
    para();
    delay(100);
  }
}
