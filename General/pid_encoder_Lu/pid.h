#define Kp 2 // P
#define Ki 0 // I
#define Kd 0 // D

#define PID Kp*e+Kd*(e-e_ant)+Ki*e_acum // calculo del PID con una macro

int e = 0, e_ant = 0, e_acum = 0; // error, error anterior y error acumulado

void error (int e1, int e2) {
  e = e1 - e2;

  if (e_ant * e > 0) {
    e_acum += e;
  } else {
    e_acum = 0;
  }

  e_ant = e;
}
