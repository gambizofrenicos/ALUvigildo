#define PIN_TRIGGER_1 4 //fuente
#define PIN_TRIGGER_2 A0
#define PIN_ECHO_2 10
#define ESPERA_ENTRE_EMISION 3 // tiempo entre lecturas consecutivas en milisegundos

#define LIM_US 5000
#define LIM_media 0.1

#define NV 21

#define FDC digitalRead(13)

#define VALOR_UMBRAL 300

#define DETECTA_LINEA sensorValues[0] > VALOR_UMBRAL

#include "encoder.h"
#include "pid.h"
#include "motores.h"
#include <QTRSensors.h>

#define NUM_SENSORS 1 // numero de sensores
#define TIMEOUT 2500 // tiempo de espera 
#define EMITTER_PIN 2 // en teoría no hace falta (es para apagar los leds)
//Del A0 al A5 son los 6 pines delanteros que siguen la linea.
//La alita I (cambios de curvatura) es el pin 10.
//La alita D (start/end) es el pin 13.
QTRSensorsRC qtrrc((unsigned char[]) {
  9
}, NUM_SENSORS, TIMEOUT, EMITTER_PIN); // Sensor QRT y pines asociados
unsigned int sensorValues[NUM_SENSORS]; // Array para guardar los valores de los sensores


long duration = 0;
int i = 0;
int v[NV] = {0};
float media = 0;
int enCasa = 1; // para detectar cuando está en el cuadrado original

int detectar() {

  digitalWrite(PIN_TRIGGER_2, LOW);
  delayMicroseconds(5);
  digitalWrite(PIN_TRIGGER_2, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIGGER_2, LOW);

  pinMode(PIN_ECHO_2, INPUT);
  duration = pulseIn(PIN_ECHO_2, HIGH);

  if (duration < LIM_US) {
    return 1;
  } else {
    return 0;
  }
}

void setup()
{
  Serial.begin(9600);
  pinMode(PIN_TRIGGER_1, OUTPUT);

  pinMode(PIN_TRIGGER_2, OUTPUT);
  pinMode(PIN_ECHO_2, INPUT);

  pinMode(13, OUTPUT);
  digitalWrite(PIN_TRIGGER_1, LOW);
  digitalWrite(13, LOW);
  delayMicroseconds(2);

  digitalWrite(PIN_TRIGGER_2, HIGH);

  pinMode(DIRD, OUTPUT);
  pinMode(DIRI, OUTPUT);
  pinMode(PWMD, OUTPUT);
  pinMode(PWMI, OUTPUT);
  // calibracion();
}

void loop() {



  if (enCasa) { // esta en la casa, busca al dragon
    // Asignar 0 y 1 al vector v en cada vuelta de loop
    v[(NV - 1) - (i % (NV - 1))] = detectar();
    //  v[i] = detectar();
    //  if (!(i % NV)) i = 0;
    i++;

    if (i > NV) {
      for (int j = 0; j < NV; j++) {
        media += v[j];
      }
      media /= NV;
    }


    if (media < LIM_media) {
      girodrag('D');
    } else if (!FDC) {
      avanzar_encoders();
    } else if (FDC) {
      enCasa = 0;
      hacia_atras();
      delay(100);
      auxi = pwmi;
      auxd = pwmd;
      pwmi = 150;
      pwmd = 150;
      avanzar();
      delay(100);
      para();
      delay(100);
      pwmi = auxi;
      pwmd = auxd;
    }
  }
  else if (!enCasa) { // no esta en la casa, vuelve al hogar
    //  hogar();
    hacia_atras();
    delay(1500);
    enCasa = 1;
  }

  //Serial.println(media);
  /* Serial.print(" ");
    Serial.println(0);*/

  media = 0;
  //  if(!FDC){
  //    avanzar_encoders();
  //  }
  //  else{
  //    hacia_atras();
  //    delay(100);
  //    auxi = pwmi;
  //    auxd = pwmd;
  //    pwmi = 150;
  //    pwmd = 150;
  //    avanzar();
  //    delay(100);
  //    para();
  //    delay(500);
  //    pwmi = auxi;
  //    pwmd = auxd;
  //    hacia_atras();
  //    delay(500);
  //  }
}

void hogar() {

  //  girar90D();
  //  girar90D();
  //  para();
  //  delay(1000);
  //  while (!DETECTA_LINEA) { //Macro aun por definir
  //    avanzar_encoders();
  //  }
  enCasa = 1;
  //  } else {
  //    avanza_mm(10); // avanza 10 mm mas para estar dentro del cuadrado
  //    enCasa = 1;
  //  }
}


