#include "pid.h"
#include "encoder.h"
#include "motores.h"
#include <QTRSensors.h>
#include <Math.h>

#define PWM_line 20
#define DETECTA_META sensorValues[7] > VALOR_UMBRAL
#define DETECTA_CURVA sensorValues[6] > VALOR_UMBRAL
#define VA_RECTO (sensorValues[2] > VALOR_UMBRAL) && (sensorValues[3] > VALOR_UMBRAL)

/****BANDERAS DE PARADA Y SALIDA****/
int start = 0;
bool parada = false;

/*******Para las curvitas******/
float reset_curva = 0; //Lo que le sumamos al pwm base (cuando es 0 va lento)


/* QTR-8RC*/
// Configuracion inicial del sensor
#define NUM_SENSORS 8 // numero de sensores
#define TIMEOUT 2500 // tiempo de espera 
#define EMITTER_PIN 2 // en teoría no hace falta (es para apagar los leds)
//Del A0 al A5 son los 6 pines delanteros que siguen la linea.
//La alita I (cambios de curvatura) es el pin 10.
//La alita D (start/end) es el pin 13.
QTRSensorsRC qtrrc((unsigned char[]) {
  A0, A1, A2, A3, A4, A5, 10, 9
}, NUM_SENSORS, TIMEOUT, EMITTER_PIN); // Sensor QRT y pines asociados
unsigned int sensorValues[NUM_SENSORS]; // Array para guardar los valores de los sensores

/* CONSTANTES */
#define VALOR_UMBRAL 500 // Valor umbral que determina si hay linea o no (hay que comprobar)

/* PID */
float errores[NUM_SENSORS] = { -3.0, -2.0, -1.0, 1.0, 2.0, 3.0, 0.0, 0.0}; // valor del error asignado a cada sensor. Las alitas no suman error

void setup() {
  /* Serial */
  Serial.begin(9600);
  Serial.println("Inicializando...");
  /* Motores */
  pinMode(DIRD, OUTPUT);
  pinMode(DIRI, OUTPUT);
  pinMode(PWMD, OUTPUT);
  pinMode(PWMI, OUTPUT);

  /* Calibración */
  //calibracion(); // calibracion del sensor y muestra por serial los datos

}

void loop() {
  /* Calculo del error */

  qtrrc.read(sensorValues);

  for (int i = 0; i < (NUM_SENSORS - 2); i++) {
    e_line += errores[i] * sensorValues[i];
  }

  error_line(); //Gestiona el error

  if (!parada) {
    pwmd = PWM_line - PID_line;
    pwmi = PWM_line + PID_line;
    acotar();
  }

  else {
    pwmd = 0;
    pwmi = 0;
  }


  //    Serial.print("PWMs:  "); Serial.print(pwmd); Serial.print("\t"); Serial.println(pwmi);
  //    Serial.print("out:  "); Serial.println(out);
  //    Serial.print("e_line:  "); Serial.println(e_line);
  //    Serial.print("start:  "); Serial.println(start);
  //    Serial.print("sensorD:  "); Serial.println(sensorValues[7]);
  //    delay(500);

}
