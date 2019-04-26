
#define GRADOS_DISPARO 100

void disparar(char canion) {


  
  /*Parar y esperar a que pare*/
  para();
  delay(1000);
  
  /*DISPAROS DE LOS CANIONES*/
  if (canion == 'D') {
    //corregir_disparo();
    while (servoD.attached() != 1) {
      servoD.attach(9);
    }
    servoD.write(GRADOS_DISPARO);
  }

  else if (canion == 'C') {
    while (servoC.attached() != 1) {
      servoC.attach(10);
    }
    servoC.write(GRADOS_DISPARO);
    //servoC.detach();
  }
  else if (canion == 'I') {
    //corregir_disparo();
    while (servoI.attached() != 1) {
      servoI.attach(11);
    }
    servoI.write(GRADOS_DISPARO);
    //servoI.detach();
    
    /*Lil giro antes de avanzar*/
//    delay(1000);
//  analogWrite(PWMI, 40);
//    analogWrite(PWMD, 0);
//    delay(80);
//    analogWrite(PWMI, 0);
//    analogWrite(PWMD, 0);


  }

  /*Parar despues de disparar*/
  delay(1000);

  
}



