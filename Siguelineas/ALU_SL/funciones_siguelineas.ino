void calibracion() {
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH); // encender LED del Arduino
  for (int i = 0; i < 400; i++) {
    qtrrc.calibrate();
  }
  digitalWrite(13, LOW); // apagar el LED

  // print the calibration minimum values measured when emitters were on
  //Serial.begin(9600);
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

/***********************************************************************/

void deteccion_meta() {
  if (!start && DETECTA_META && !out) {
    start = 1;
  }

  else if ((start == 1) && !(DETECTA_META) && !out) {
    start = 2;
  }

  if ((start == 2) && DETECTA_META && !out) {
    parada = true;
  }
}

/***********************************************************************/
bool after_cruce = false;
bool sobrecurva_aftcruce = false;

void deteccion_curva() {
  //El sensor lee una marca de curva
  if (DETECTA_CURVA && !after_cruce) {
    reset_curva = 0;
  }
  //El sensor esta leyendo el cruce, no es una curva
  else if (DETECTA_CURVA && after_cruce) {
    sobrecurva_aftcruce = true;
  }

  //El sensor ya ha salido del cruce y resetea las variables pertinentes
  else if (!DETECTA_CURVA && sobrecurva_aftcruce) {
    sobrecurva_aftcruce = false;
    after_cruce = false;
  }
}

/***********************************************************************/

int horizontal = 0;

void deteccion_cruce() {

  for (unsigned char i = 0; i < (NUM_SENSORS - 2); i++) {
    if (sensorValues[i] > VALOR_UMBRAL) horizontal = 1;
    else {
      horizontal = 0;
      break;
    }
  }

  if (horizontal) { //Cuando haya detectado la horizontal
    start = 0; //Reiniciamos la meta (alitaD)
    after_cruce = true; //Y aaqui
  }
}

