#include <Servo.h>
#include <QTRSensors.h>
#include <Math.h>
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

/*********VARIABLES DEL FINAL DE CARRERA****************/
#define FDC 13

/*********VARIABLES DEL PROGRAMA****************/
int fase = 0;

void setup() {
  //Incluimos los motores+encoders
  inicializar_motores();
  
  //Incluimos el final de carrera
  pinMode(FDC, INPUT);

  //Calibramos siguelineas
  calibrar();

  //Metodo de salida (pulsando el FDC con la tabla)
  salida();

  delayMicroseconds(500000); //Esperamos medio segundo despues de que se de la salida
  fase=1;
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
      Serial.println("FLANCO 1");
      disparar('C');
      fase=2;

    }
    horiz_anterior = horiz_actual;
  }
  
  /****FASE 2****/
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
    //Avanzar recto hasta que toca final de carrera
    while(!digitalRead(FDC)){
      avanzar_encoders();
    }
    para();
    
  }



}

