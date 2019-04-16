
#define GRADOS_DISPARO 100

void disparar(char canion){
  if(canion=='D') servoD.write(GRADOS_DISPARO);
  else if(canion=='C') servoC.write(GRADOS_DISPARO);
  else if(canion=='I') servoI.write(GRADOS_DISPARO);
}



