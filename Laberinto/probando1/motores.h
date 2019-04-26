// PWM
#define PWM 40 // PWM "base" a los motores (al que queremos que vayan)
#define MAX_PWM 150 // Limitacion superior de PWM
#define MIN_PWM 10 // Limitacion inferior de PWM


// Giro
#define G90I 108
#define G90D -108

// Pulsos por vuelta
#define PPV 180

// Motor izquierdo
#define PWMI 5
#define DIRI 7

//Motor derecho
#define PWMD 6
#define DIRD 8

float pwmi = 0, pwmd = 0; // pwm que pasamos a cada motor

void acotar();
void arrancar();
void avanzar();
void girar90I();
void girar90D();
void girar180();
void girar90D_coche();
void para();
void avanza_mm(float d);
void avanza_mm_lab(float d);

//Arrancar sin caballito
void arrancar(int e1, int e2) {

  digitalWrite(DIRI, HIGH);
  digitalWrite(DIRD, LOW);

  for (int i = MIN_PWM; i <= PWM; i++) {

    error(e1, e2);

    pwmi = i - e;
    pwmd = i + e;

    acotar();

    analogWrite(PWMI, pwmi);
    analogWrite(PWMD, pwmd);
    delay(10);
  }
}

// Acotar el pwm de los motores para que no se salga del MIN y MAX
void acotar() {
  if (pwmi > MAX_PWM) {
    pwmi = MAX_PWM;
  } else if (pwmi < MIN_PWM) {
    pwmi = MIN_PWM;
  }

  if (pwmd > MAX_PWM) {
    pwmd = MAX_PWM;
  } else if (pwmd < MIN_PWM) {
    pwmd = MIN_PWM;
  }
}

void avanzar() {
  digitalWrite(DIRI, HIGH);
  digitalWrite(DIRD, LOW);

  analogWrite(PWMI, pwmi);
  analogWrite(PWMD, pwmd);
}

void girar90I() {
  CountD = 0;
  CountI = 0;

  e = 0;

  error(G90I, (CountD - CountI) / 2);

  while (e) {
    if (e > 0) {
      digitalWrite(DIRI, LOW);
      digitalWrite(DIRD, LOW);
    } else if (e < 0) {
      digitalWrite(DIRI, HIGH);
      digitalWrite(DIRD, HIGH);
    }


    pwmi = PWM + PID_g;
    pwmd = PWM + PID_g;

    acotar();

    analogWrite(PWMI, pwmi);
    analogWrite(PWMD, pwmd);

    error(G90I, (CountD - CountI) / 2);

   /* Serial.print("e_I:\t");
    Serial.print(e);
    Serial.print("\tPID_I:\t");
    Serial.println(PID_g);*/


  }
}

void girar90D() {

  CountD = 0;
  CountI = 0;

  error(G90D, (CountD - CountI) / 2);

  while (e) {
    if (e < 0) {
      digitalWrite(DIRI, HIGH);
      digitalWrite(DIRD, HIGH);
    } else if (e > 0) {
      digitalWrite(DIRI, LOW);
      digitalWrite(DIRD, LOW);
    }

    pwmi = PWM - PID_g;
    pwmd = PWM - PID_g;

    acotar();

    analogWrite(PWMI, pwmi);
    analogWrite(PWMD, pwmd);

    error(G90D, (CountD - CountI) / 2);

  /*  Serial.print("e_D:\t");
    Serial.print(e);
    Serial.print("\tPID_D:\t");
    Serial.println(PID_g);*/



  }

}

void girar180() {
  CountD = 0;
  digitalWrite(DIRI, HIGH);
  digitalWrite(DIRD, HIGH);
  while (2*G90D < CountD) {
    analogWrite(PWMI, PWM);
    analogWrite(PWMD, PWM);
  }
}

void girar90D_coche(){
  CountI = 0;
  digitalWrite(DIRI, HIGH);
  digitalWrite(DIRD, HIGH);
  while (281 > CountI) {
    Serial.println(CountI);
    analogWrite(PWMI, PWM);
    analogWrite(PWMD, 0);
  }
}

void para() {
  analogWrite(PWMI, 0);
  analogWrite(PWMD, 0);
}

void avanza_mm(float d) {
  int encI = CountI;
  int encD = CountD;

  d = (d/(2*PI*16)) * PPV; //16 mm es el radio de las ruedas de pololu
  error_mm(d,((CountI + CountD)/2) - ((encI + encD)/2));
  
  //while ((((CountI + CountD) / 2) - ((encI + encD) / 2)) < d) {
  while ((e_mm >= 1) || (e_mm <= -1)){
    
  error(CountI, CountD);
	error_mm(d,((CountI + CountD)/2) - ((encI + encD)/2));

    pwmi = PID_mm - PID;
    pwmd = PID_mm + PID;

    acotar();
    avanzar();
  }

}

void avanza_mm_lab(float d) {
  int encI = CountI;
  int encD = CountD;

  d = (d/(2*PI*16)) * PPV; //16 mm es el radio de las ruedas de pololu
  error_mm(d,((CountI + CountD)/2) - ((encI + encD)/2));
  
  //while ((((CountI + CountD) / 2) - ((encI + encD) / 2)) < d) {
  while ((e_mm >= 1) || (e_mm <= -1)){
    
  error((2076.0 / (analogRead(SHARPI) - 11.0)), (2076.0 / (analogRead(SHARPO) - 11.0)));
  error_mm(d,((CountI + CountD)/2) - ((encI + encD)/2));

    pwmi = PWM - PID_lab;
    pwmd = PWM + PID_lab;

    acotar();
    avanzar();
  }

}
