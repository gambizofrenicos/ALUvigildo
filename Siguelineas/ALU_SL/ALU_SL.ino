#include "pid.h"
#include "encoder.h"
#include "motores.h"
#include <QTRSensors.h>
#include <Math.h>

#define PWM_line 80
#define DETECTA_META sensorValues[7] > VALOR_UMBRAL
#define DETECTA_CURVA sensorValues[6] > VALOR_UMBRAL
#define VA_RECTO (sensorValues[2] > VALOR_UMBRAL) && (sensorValues[3] > VALOR_UMBRAL)

/* QTR-8RC*/
// Configuracion inicial del sensor
#define NUM_SENSORS 8 // numero de sensores
#define TIMEOUT 2500 // tiempo de espera 
#define EMITTER_PIN 2 // en teoría no hace falta (es para apagar los leds)
//Del A0 al A5 son los 6 pines delanteros que siguen la linea.
//La alita I (cambios de curvatura) es el pin 10.
//La alita D (start/end) es el pin 13.
QTRSensorsRC qtrrc((unsigned char[]) {
  A0, A1, A2, A3, A4, A5, 10, 13
}, NUM_SENSORS, TIMEOUT, EMITTER_PIN); // Sensor QRT y pines asociados
unsigned int sensorValues[NUM_SENSORS]; // Array para guardar los valores de los sensores

unsigned int sensores_detectando = 0;

int out = 0;

/* CONSTANTES */
#define VALOR_UMBRAL 500 // Valor umbral que determina si hay linea o no (hay que comprobar)
#define VNOM 200 // velocidad nominal de los motores

/* PID */
float errores[NUM_SENSORS] = { -3.0, -2.0, -1.0, 1.0, 2.0, 3.0, 0.0, 0.0}; // valor del error asignado a cada sensor. Las alitas no suman error

void setup() {
  /* Serial */
  Serial.begin(9600);

  /* Motores */
  pinMode(DIRD, OUTPUT);
  pinMode(DIRI, OUTPUT);
  pinMode(PWMD, OUTPUT);
  pinMode(PWMI, OUTPUT);

  /* Calibración */
  calibracion(); // calibracion del sensor y muestra por serial los datos

}

void loop() {
  /* Calculo del error */

  qtrrc.read(sensorValues);

  /*if (DETECTA_META) {
    //Ha llegado a la meta, parese wey
    //para();
    }
    else if (DETECTA_CURVA) {
    //Hay curva
    }
    else {*/
  for (int i = 0; i < (NUM_SENSORS - 2); i++) {
    if (sensorValues[i] > VALOR_UMBRAL) {
      // Si el sensor supera un cierto umbral, se suma su error asignado
      e_line += errores[i];
      sensores_detectando++;
      out = 0;
    }
  }

  e_line = e_line / sensores_detectando;
  if (e_line == 3) out = 1;
  else if (e_line == -3) out = -1;
  if (!sensores_detectando) sensores_detectando = 1;

  Serial.println(e_line);
  error_line();

  if ((out > 0)&&(sensorValues[5] < VALOR_UMBRAL)){
    //Te has salio por la izda maskina 
    e_line = 3;
  }
  else if((out < 0)&&(sensorValues[0] < VALOR_UMBRAL)){
    //Te has salio por la dcha maskina
    e_line = -3;
  }
  pwmd = PWM_line - PID_line;
  pwmi = PWM_line + PID_line;

  acotar();

  e_line = 0;
  sensores_detectando = 0;

  /* Mover los motores */

  //  Serial.print(pwmd); Serial.print("\t"); Serial.println(pwmi);
  //  delay(200);
  digitalWrite(DIRD, LOW);
  digitalWrite(DIRI, HIGH);
  analogWrite(PWMD, pwmd);
  analogWrite(PWMI, pwmi);
  //}


  /* Calculo de las velocidades de cada motor */
  /*
     e < 0 --> girar a derecha --> vDer aumenta --> como PID<0 (porque e<0) --> vDer = VNOM - PID%56 (para sumar o restar de 0 a 55)
     e > 0 --> girar a izquierda --> vDer disminuye --> como PID>0 (porque e>0) --> vDer = VNOM - PID%56 (para sumar o restar de 0 a 55)
  */


  /* Asignacion del error anterior y suma del error acumulado */
}

void calibracion() {
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH); // encender LED del Arduino
  for (int i = 0; i < 400; i++) {
    qtrrc.calibrate();
  }
  digitalWrite(13, LOW); // apagar el LED

  // print the calibration minimum values measured when emitters were on
  Serial.begin(9600);
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtrrc.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();

  // print the calibration maximum values measured when emitters were on
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtrrc.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(500);
}

