// PWM
#define PWM 40 // PWM "base" a los motores (al que queremos que vayan)
#define MAX_PWM 100 // Limitacion superior de PWM
#define MIN_PWM 20 // Limitacion inferior de PWM


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

float pwmi = 0, pwmd = 0; // pwm que pasamos a cada motor

void inicializar_motores();
void acotar();
void arrancar();
void avanzar();
void avanzar_encoders();
void girar90I();
void girar90D();
void girar90D_coche();
void para();
void avanza_mm(float d);


//Inicializar motores y encoders
void inicializar_motores() {
  // Inicializamos motor I
  pinMode(ENC1_MOTI, INPUT);
  pinMode(ENC2_MOTI, INPUT);
  pinMode(PWMI, OUTPUT);
  pinMode(DIRI, OUTPUT);

  // Inicializamos motor D
  pinMode(ENC1_MOTD, INPUT);
  pinMode(ENC2_MOTD, INPUT);
  pinMode(PWMD, OUTPUT);
  pinMode(DIRD, OUTPUT);

  // initialize hardware interrupts
  attachInterrupt(0, EncoderEventMotI_ROTO, CHANGE);
  attachInterrupt(1, EncoderEventMotD, CHANGE);
}

//Arrancar sin caballito
void arrancar() {

  digitalWrite(DIRI, HIGH);
  digitalWrite(DIRD, LOW);

  for (int i = MIN_PWM; i <= PWM; i++) {

    error(CountI, CountD);

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

void avanzar_encoders() {
  error(CountI, CountD);

  pwmi = PWM - PID;
  pwmd = PWM + PID;

  acotar();

  avanzar();
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

void girar90D_coche() {
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

  d = (d / (2 * PI * 16)) * PPV; //16 mm es el radio de las ruedas de pololu
  error_mm(d, ((CountI + CountD) / 2) - ((encI + encD) / 2));

  //while ((((CountI + CountD) / 2) - ((encI + encD) / 2)) < d) {
  while ((e_mm >= 1) || (e_mm <= -1)) {

    error(CountI, CountD);
    error_mm(d, ((CountI + CountD) / 2) - ((encI + encD) / 2));

    pwmi = PID_mm - PID;
    pwmd = PID_mm + PID;

    acotar();
    avanzar();
  }

}
