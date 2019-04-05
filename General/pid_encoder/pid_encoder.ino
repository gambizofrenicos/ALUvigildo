// Velocidad

#define MAX_PWM 255 // Limitacion superior de PWM
#define MIN_PWM 30 // Limitacion inferior de PWM

// Pines motor izquierdo + encoder (malito)
#define ENC1_MOTI 2
#define ENC2_MOTI 4
#define PWMI 5
#define DIRI 7

// Pines motor derecho + encoder
#define ENC1_MOTD 3
#define ENC2_MOTD 12
#define PWMD 6
#define DIRD 8

// PID
int e = 0, e_ant = 0, e_acum = 0; // error, error anterior y error acumulado
int pwmi = 0, pwmd = 0; // pwm que pasamos a cada motor


#define Kp 1 // P
#define Ki 0 // I
#define Kd 0 // D
#define PID Kp*e+Kd*(e-e_ant)+Ki*e_acum // calculo del PID con una macro


// variables to store the number of encoder pulses

volatile long CountI = 0;
volatile long CountD = 0;

void setup() {

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
  attachInterrupt(0, EncoderEventMotI, CHANGE);
  attachInterrupt(1, EncoderEventMotD, CHANGE);

  Serial.begin(9600);

  digitalWrite(DIRI, HIGH);
  digitalWrite(DIRD, LOW);
}

void loop() {

  PID_recto(50);
  delay(200);
  PID_recto(80);
  delay(200);
  PID_recto(150);
  delay(200);
  PID_recto(250);
  delay(100);
  analogWrite(PWMI, 0);
  analogWrite(PWMD, 0);
  delay(5000);
}

void PID_recto(int PWM){

  // PWM: "base" a los motores (al que queremos que vayan)
   
  e = CountI - CountD;

  //Reseteamos el e_acum cuando pasa por cero. Sumamos el error al error acumulado
  if (e_ant * e > 0) {
    e_acum += e;
  } else {
    e_acum = 0;
  }

  e_ant = e;

  pwmi = PWM - PID;
  pwmd = PWM + PID;

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

  analogWrite(PWMI, pwmi);
  analogWrite(PWMD, pwmd);
}

// Funcion encoder motor izquierdo
void EncoderEventMotI() {
  if (digitalRead(ENC1_MOTI) == HIGH) {
    if (digitalRead(ENC2_MOTI) == LOW) {
      CountI--;
    } else {
      CountI++;
    }
  } else {
    if (digitalRead(ENC2_MOTI) == LOW) {
      CountI++;
    } else {
      CountI--;
    }
  }
}

// Funcion encoder motor derecho
void EncoderEventMotD() {
  if (digitalRead(ENC1_MOTD) == HIGH) {
    if (digitalRead(ENC2_MOTD) == LOW) {
      CountD++;
    } else {
      CountD--;
    }
  } else {
    if (digitalRead(ENC2_MOTD) == LOW) {
      CountD--;
    } else {
      CountD++;
    }
  }
}
