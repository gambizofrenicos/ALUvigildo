#define PWM_line 60
#define DETECTA_META sensorValues[7] > VALOR_UMBRAL
#define DETECTA_CURVA sensorValues[6] > VALOR_UMBRAL
#define VA_RECTO (sensorValues[2] > VALOR_UMBRAL) && (sensorValues[3] > VALOR_UMBRAL)

unsigned int sensores_detectando = 0;

int out = 0;

/* CONSTANTES */
#define VALOR_UMBRAL 500 // Valor umbral que determina si hay linea o no (hay que comprobar)
#define VNOM 200 // velocidad nominal de los motores

float errores[NUM_SENSORS] = { -3.0, -2.0, -1.0, 1.0, 2.0, 3.0}; // valor del error asignado a cada sensor. Las alitas no suman error


/****************************************************************************************************/
void salida() {
  while (fase == 0) { //While para esperar a que se pulse
    //Esperamos a que sea 1
    if (digitalRead(FDC) == 1) {
      fase = 1;
      Serial.println("PULSAO");
    }
    delay(10);
  }

  while (fase == 1) { //While para esperar a que se deje de pulsar
    if (digitalRead(FDC) == 0) {
      fase = 0;
      Serial.println("DESPULSAO");
    }
    delay(10);
  }
}

/****************************************************************************************************/

void calibrar() {
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
}


/****************************************************************************************************/


void seguir_linea()
{
  /* Calculo del error */
  qtrrc.read(sensorValues);

  /*else if (DETECTA_CURVA) {
    //Hay curva
    }
    else {*/
  for (int i = 0; i < (NUM_SENSORS); i++) {
    if (sensorValues[i] > VALOR_UMBRAL) {
      // Si el sensor supera un cierto umbral, se suma su error asignado
      e_line += errores[i];
      sensores_detectando++;
      out = 0;
    }
  }

  if (sensores_detectando) e_line = e_line / sensores_detectando;

  if (e_line == 3) out = 1;
  else if (e_line == -3) out = -1;

  //Serial.println(e_line);

  if ((out > 0) && (sensorValues[5] < VALOR_UMBRAL)) {
    //Te has salio por la izda maskina
    e_line = 3;
  }
  else if ((out < 0) && (sensorValues[0] < VALOR_UMBRAL)) {
    //Te has salio por la dcha maskina
    e_line = -3;
  }

  error_line(); //Gestiona el error


  pwmd = PWM_line - PID_line;
  pwmi = PWM_line + PID_line;

  acotar();


  e_line = 0;
  sensores_detectando = 0;

  digitalWrite(DIRD, LOW);
  digitalWrite(DIRI, HIGH);
  analogWrite(PWMD, pwmd);
  analogWrite(PWMI, pwmi);
}




/****************************************************************************************************/

int horizontal = 0;
int detectar_horizontal() {
  for (unsigned char i = 0; i < NUM_SENSORS; i++) {
    if (sensorValues[i] > 800) horizontal = 1;
    else {
      horizontal = 0;
      break;
    }
  }
  return horizontal;
}

