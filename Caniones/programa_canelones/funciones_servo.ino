
#define GRADOS_DISPARO 100

void disparar(char canion) {

  while (servoC.attached() != 1) {
    servoC.attach(10);
  }


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
    servoC.detach();
  }
  else if (canion == 'I') {
    while (servoI.attached() != 1) {
      servoI.attach(11);
    }
    servoI.write(GRADOS_DISPARO);
    servoI.detach();
  }

}



