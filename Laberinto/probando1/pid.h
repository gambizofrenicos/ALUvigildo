#define  Kp 2 // P
#define  Ki 0 // I
#define  Kd 0 // D

#define  Kp_g 0.1 // P
#define  Ki_g 0 // I
#define  Kd_g 0 // D

#define  Kp_lab  8 // P
#define  Ki_lab  0 // I
#define  Kd_lab  5 // D

#define  Kp_mm  1
#define  Ki_mm  0
#define  Kd_mm  0

#define  PID Kp*e+Kd*(e-e_ant)+Ki*e_acum // calculo del PID con una macro
#define  PID_g Kp_g*e+Kd_g*(e-e_ant)+Ki_g*e_acum // calculo del PID con una macro
#define  PID_mm  Kp_mm*e_mm+Kd_mm*(e_mm-e_ant_mm)+Ki_mm*e_acum_mm // calculo del PID con una macro
#define  PID_lab  Kp_lab*e+Kd_lab*(e-e_ant)+Ki_lab*e_acum // calculo del PID con una macro

float e = 0, e_ant = 0, e_acum = 0; // error, error anterior y error acumulado
float e_mm = 0.0, e_ant_mm = 0.0, e_acum_mm = 0.0; // error, error anterior y error acumulado


void error (float e1, float e2) {
  e = e1 - e2;

  if (e_ant * e > 0) {
    e_acum += e;
  } else {
    e_acum = 0;
  }

  e_ant = e;
}

void error_mm (int e1, int e2) {
  e_mm = e1 - e2;

  if (e_ant_mm * e_mm > 0) {
    e_acum_mm += e_mm;
  } else {
    e_acum_mm = 0;
  }

  e_ant_mm = e_mm;
}
