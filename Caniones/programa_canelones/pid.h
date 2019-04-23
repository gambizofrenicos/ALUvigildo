#define Kp 2 // P
#define Ki 0 // I
#define Kd 0 // D

#define Kp_lab 10 // P
#define Ki_lab 0 // I
#define Kd_lab 2 // D

#define Kp_mm 1
#define Kd_mm 0
#define Ki_mm 0

#define Kp_line 6
#define Kd_line 60
#define Ki_line 0

#define PID Kp*e+Kd*(e-e_ant)+Ki*e_acum // calculo del PID con una macro
#define PID_mm Kp_mm*e_mm+Kd_mm*(e_mm-e_ant_mm)+Ki_mm*e_acum_mm // calculo del PID con una macro
#define PID_lab Kp_lab*e+Kd_lab*(e-e_ant)+Ki_lab*e_acum // calculo del PID con una macro
#define PID_line Kp_line*e_line+Kd_line*(e_line-e_ant_line)+Ki_line*e_acum_line // calculo del PID con una macro

float e = 0, e_ant = 0, e_acum = 0; // error, error anterior y error acumulado
float e_mm = 0.0, e_ant_mm = 0.0, e_acum_mm = 0.0; // error, error anterior y error acumulado
float e_line = 0.0, e_ant_line = 0.0, e_acum_line = 0.0; // error, error anterior y error acumulado


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

void error_line () {
  
  if (e_ant_line * e_line > 0) {
    e_acum_line += e_line;
  } else {
    e_acum_line = 0;
  }
  e_ant_line = e_line;
}
