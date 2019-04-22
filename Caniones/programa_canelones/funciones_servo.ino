
#define GRADOS_DISPARO 100

void disparar(char canion) {
  
  /*Parar y esperar a que pare*/
  para();
  delay(1000);
  
  /*DISPAROS DE LOS CANIONES*/
  if (canion == 'D') {
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
    while (servoI.attached() != 1) {
      servoI.attach(11);
    }
    servoI.write(GRADOS_DISPARO);
    //servoI.detach();
  }

  /*Parar despues de disparar*/
  delay(1000);

}



