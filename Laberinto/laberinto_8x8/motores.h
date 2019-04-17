// PWM
#define PWM 95 // PWM "base" a los motores (al que queremos que vayan)
#define MAX_PWM 150 // Limitacion superior de PWM
#define MIN_PWM 40 // Limitacion inferior de PWM


// Giro
#define G90I 130
#define G90D -100

// Pulsos por vuelta
#define PPV 180

// Motor izquierdo
#define PWMI 5
#define DIRI 7

//Motor derecho
#define PWMD 6
#define DIRD 8

//Sharps
#define SHARPI A0
#define SHARPA 10
#define SHARPO A1

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
  digitalWrite(DIRI, LOW);
  digitalWrite(DIRD, LOW);
  while (G90I > CountD) {
    analogWrite(PWMI, PWM);
    analogWrite(PWMD, PWM);
  }
}

void girar90D() {
  CountD = 0;
  digitalWrite(DIRI, HIGH);
  digitalWrite(DIRD, HIGH);
  while (G90D < CountD) {
    analogWrite(PWMI, PWM);
    analogWrite(PWMD, PWM);
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
