#include <Servo.h>
#include <QTRSensors.h>
#include "pid.h"
#include "encoder.h"
#include "motores.h"

/*********VARIABLES DEL SIGUELINEAS***************/
#define NUM_SENSORS   6     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   2     // emitter is controlled by digital pin 2

QTRSensorsRC qtrrc((unsigned char[]) {
  A0, A1, A2, A3, A4, A5
}, NUM_SENSORS, TIMEOUT, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];

/*********VARIABLES DE LOS SERVOS****************/
Servo servoI;  // create servo object to control a servo
Servo servoC;  // create servo object to control a servo
Servo servoD;  // create servo object to control a servo

/*********VARIABLES DEL PROGRAMA****************/
int fase = 0;

void setup() {
  //Incluimos los servos
  servoI.attach(9);
  servoC.attach(10);
  servoD.attach(11);

  //Calibramos siguelineas
  for (int i = 0; i < 200; i++)  // make the calibration take about 10 seconds
  {
    qtrrc.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
  }

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
  delay(1000);

  //Empezamos el programa en la fase 1 (leer la linea recta)
  fase = 1;
}

int horiz_actual = 0;
int horiz_anterior = 0;

void loop() {

  /****FASE 1****/
  //Leemos la linea para ir en linea recta y cuando se pare, paramos
  if (fase == 1) {
    leer_linea();
    //Aqui ira la funcion de seguir linea

    //Cuando detecta horizontal para y dispara y cambiamos de fase
    horiz_actual = detectar_horizontal();
    if (horiz_actual == 1 && horiz_anterior == 0) {
      para();
      disparar('C');
      fase=2;
      Serial.println("FLANCO 1");
    }
    horiz_anterior = horiz_actual;
  }

  else if (fase==2){
    leer_linea();
    //Avanzar recto el roboti

    //Cuando detecta horizontal para y dispara y cambiamos de fase
    horiz_actual = detectar_horizontal();
    if (horiz_actual == 1 && horiz_anterior == 0) {
      para();
      disparar('D');
      fase=3;
      Serial.println("FLANCO 2");
    }
    horiz_anterior = horiz_actual;
  }
  
  else if (fase==3){
    leer_linea();
    //Avanzar recto el roboti

    //Cuando detecta horizontal para y dispara y cambiamos de fase
    horiz_actual = detectar_horizontal();
    if (horiz_actual == 1 && horiz_anterior == 0) {
      para();
      disparar('I');
      fase=4;
      Serial.println("FLANCO 3");
    }
    horiz_anterior = horiz_actual;
  }
  
  else if (fase==4){
    //Avanzar recto el roboti unos 60mm hasta la torre
    
    //Cuando detecta el final de carrera para definitivamente
    horiz_actual = detectar_horizontal();
    if (horiz_actual == 1 && horiz_anterior == 0) {
      para();
      disparar('D');
      fase=3;
      Serial.println("FLANCO 2");
    }
    horiz_anterior = horiz_actual;
  }



}

