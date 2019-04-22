#define PIN_TRIGGER_1 4 //fuente
#define PIN_TRIGGER_2 A0
#define PIN_ECHO_2 10
#define ESPERA_ENTRE_EMISION 3 // tiempo entre lecturas consecutivas en milisegundos

#define LIM_US 1500
#define LIM_media 0.01

#define NV 21

#define FDC digitalRead(13)

#include "encoder.h"
#include "pid.h"
#include "motores.h"

long duration = 0;
int i = 0;
int v[NV] = {0};
float media = 0;

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
}

void loop() {

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
  } else if(!FDC){
    avanzar_encoders();
  }
  else {
    hogar();
  }

  //Serial.println(media);
 /* Serial.print(" ");
  Serial.println(0);*/

  media = 0;
}

void hogar(){
  giro180();
  if (!DETECTA_LINEA) avanzar_encoders();//Macro aun por definir
}


