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

void deteccion_meta() {
  if (!start && DETECTA_META) {
    start = 1;
  }

  else if ((start == 1) && !(DETECTA_META)) {
    start = 2;
  }

  if ((start == 2) && DETECTA_META) {
    parada = true;
  }
}

void deteccion_cruce() {
  
  for(unsigned char i=0; i<NUM_SENSORS; i++){
    if(sensorValues[i] > VALOR_UMBRAL) horizontal=1;
    else{
      horizontal=0;
      break;
    }
  }
  
  
}

