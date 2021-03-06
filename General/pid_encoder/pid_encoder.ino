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

  // initialize hardware interrupts
  attachInterrupt(0, EncoderEventMotI_ROTO, CHANGE);
  attachInterrupt(1, EncoderEventMotD, CHANGE);

  Serial.begin(9600);

//  arrancar(CountI, CountD);
}

void loop() {

  /*error(CountI, CountD);

    pwmi = PWM - PID;
    pwmd = PWM + PID;

    acotar();

    avanzar();

    girar90I();

    para();

    delay(1000);

    girar90D();*/

  /*   int d = 168;
    int encI = CountI;
    int encD = CountD;
    while ((((CountI + CountD) / 2) - ((encI + encD) / 2)) < d) {
     avanza_mm(d);
    }*/
//  arrancar(CountI, CountD);
  avanzar_encoders();
 // delay(1000);

   Serial.print(CountD);
    Serial.print("\t");
    Serial.print(CountI);
    Serial.print("\t");
    Serial.print(digitalRead(ENC1_MOTI));
    Serial.print("\t");
    Serial.println(digitalRead(ENC2_MOTI));

 /*   Serial.print("CountI:\t");
    Serial.print(CountI);
    Serial.print("\t\t");
    Serial.print("CountD:\t");
    Serial.println(CountD);*/


}
