#define PWM 40 // PWM "base" a los motores (al que queremos que vayan)
#define MAX_PWM 150 // Limitacion superior de PWM
#define MIN_PWM 0 // Limitacion inferior de PWM

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

#define SHARPI A0
#define SHARPA 10
#define SHARPO A1

float dist_i;
float dist_d;

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

}

void loop() {
  dist_i = 2076.0/(analogRead(SHARPI)-11.0);
  dist_d = 2076.0/(analogRead(SHARPO)-11.0);
  digitalWrite(DIRI, HIGH);
  digitalWrite(DIRD, LOW);
  if ((dist_i < 8)){
    analogWrite(PWMI, PWM);
    } else {
      analogWrite(PWMI, MIN_PWM);
      }
  if ((dist_d < 8)){
    analogWrite(PWMD, PWM);
    } else {
      analogWrite(PWMD, MIN_PWM);
      }
}
