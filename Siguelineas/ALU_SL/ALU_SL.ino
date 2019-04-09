#include <QTRSensors.h>
#include <Math.h>


/* QTR-8RC*/
// Configuracion inicial del sensor
#define NUM_SENSORS 8 // numero de sensores
#define TIMEOUT 2500 // tiempo de espera 
#define EMITTER_PIN 2 // en teoría no hace falta (es para apagar los leds)
QTRSensorsRC qtrrc((unsigned char[]) {
  A0, A1, A2, A3, A4, A5, A6, A7
}, NUM_SENSORS, TIMEOUT, EMITTER_PIN); // Sensor QRT y pines asociados
unsigned int sensorValues[NUM_SENSORS]; // Array para guardar los valores de los sensores

/* CONSTANTES */
#define VALOR_UMBRAL 500 // Valor umbral que determina si hay linea o no (hay que comprobar)
#define VNOM 200 // velocidad nominal de los motores

/* PID */
int errores[NUM_SENSORS] = { -4, -3, -2, -1, 1, 2, 3, 4}; // valor del error asignado a cada sensor
int e, e_ant, e_acum; // error, error anterior y error acumulado
#define Kp 1 // P
#define Ki 0 // I
#define Kd 0 // D
#define PID Kp*e+Kd*(e-e_ant)+Ki*e_acum // calculo del PID con un define

/* MOTORES */
#define dirDer 8 // pin de direccion del motor derecho
#define dirIzq 9 // pin de direccion del motor izquierdo
#define PWMDer 5 // pin de pwm del motor derecho
#define PWMIzq 6 // pin de pwm del motor izquierdo
int vDer, vIzq; // velocidades derecha e izquierda

void setup() {
  /* Serial */
  Serial.begin(9600);

  /* Motores */
  pinMode(dirDer, OUTPUT);
  pinMode(dirIzq, OUTPUT);
  pinMode(PWMDer, OUTPUT);
  pinMode(PWMIzq, OUTPUT);

  /* Calibración */
  calibracion(); // calibracion del sensor y muestra por serial los datos

}

void loop() {
  /* Calculo del error */
  for (int i = 0; i < NUM_SENSORS; i++) {
    if (sensorValues[i] > VALOR_UMBRAL) {
      // Si el sensor supera un cierto umbral, se suma su error asignado
      e += errores[i];
    }
  }

  /* Calculo de las velocidades de cada motor */
  /*
     e < 0 --> girar a derecha --> vDer aumenta --> como PID<0 (porque e<0) --> vDer = VNOM - PID%56 (para sumar o restar de 0 a 55)
     e > 0 --> girar a izquierda --> vDer disminuye --> como PID>0 (porque e>0) --> vDer = VNOM - PID%56 (para sumar o restar de 0 a 55)
  */
  vDer = VNOM - PID % 56;
  vIzq = VNOM + PID % 56;

  /* Mover los motores */
  digitalWrite(dirDer, HIGH);
  digitalWrite(dirIzq, HIGH);
  analogWrite(PWMDer, vDer);
  analogWrite(PWMIzq, vIzq);

  /* Asignacion del error anterior y suma del error acumulado */
  e_ant = e;
  if (e_ant * e > 0) {
    e_acum += e;
  } else {
    e_acum = 0;
  }
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

