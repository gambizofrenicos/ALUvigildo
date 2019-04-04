/*
   Encoder example sketch
   by Andrew Kramer
   1/1/2016

   Records encoder ticks for each wheel
   and prints the number of ticks for
   each encoder every 500ms

*/

// pins for the encoder inputs
#define ENC1_MOTA 2
#define ENC2_MOTA 4
#define ENC1_MOTB 3
#define ENC2_MOTB 12
#define PWMA 5
#define PWMB 6
#define DIRA 7
#define DIRB 8

// variables to store the number of encoder pulses
// for each motor
volatile long countA = 0;
volatile long countB = 0;

void setup() {
  pinMode (ENC1_MOTA, INPUT);
  pinMode (ENC2_MOTA, INPUT);
  pinMode (ENC1_MOTB, INPUT);
  pinMode (ENC2_MOTB, INPUT);
  pinMode (PWMA, OUTPUT);
  pinMode (DIRA, OUTPUT);
  pinMode (PWMB, OUTPUT);
  pinMode (DIRB, OUTPUT);

  // initialize hardware interrupts
  attachInterrupt(0, EncoderEventMotA, CHANGE);
  attachInterrupt(1, EncoderEventMotB, CHANGE);

  Serial.begin(9600);
}

void loop() {
  Serial.println(countA);
  /*if (countA <= 32) {
    digitalWrite (DIRA, LOW);
    analogWrite (PWMA, 60);
  }
  else {
    analogWrite (PWMA, 0);
    delay (2000);
    countA = 0;
  }*/

  //  digitalWrite(DIRA, HIGH);
  //  analogWrite(PWMA, 100);
  //  digitalWrite(DIRB, HIGH);
  //  analogWrite(PWMB, 100);
  //  delay(500);
  //  Serial.print("Count: ");
  //  Serial.println(Count);
  //  analogWrite(PWMA, 0);
  //  analogWrite(PWMB, 0);
  //  delay(500);
  //  digitalWrite(DIRA, LOW);
  //  analogWrite(PWMA, 100);
  //  digitalWrite(DIRB, LOW);
  //  analogWrite(PWMB, 100);
  //  delay(500);
  //  Serial.print("Count: ");
  //  Serial.println(Count);
  //  analogWrite(PWMA, 0);
  //  analogWrite(PWMB, 0);
  //  delay(500);
}

// MOTOR IZQUIERDO
void EncoderEventMotA() {
  if (digitalRead(ENC1_MOTA) == HIGH) {
    if (digitalRead(ENC2_MOTA) == LOW) {
      countA--;
    } else {
      countA++;
    }
  } else {
    if (digitalRead(ENC2_MOTA) == LOW) {
      countA++;
    } else {
      countA--;
    }
  }
}

void EncoderEventMotB() {
  if (digitalRead(ENC1_MOTB) == HIGH) {
    if (digitalRead(ENC2_MOTB) == LOW) {
      countB++;
    } else {
      countB--;
    }
  } else {
    if (digitalRead(ENC2_MOTB) == LOW) {
      countB--;
    } else {
      countB++;
    }
  }
}
