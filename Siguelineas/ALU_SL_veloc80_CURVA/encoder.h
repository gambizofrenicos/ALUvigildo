// Encoder izquierdo (malito)
#define ENC1_MOTI 2
#define ENC2_MOTI 4


// Encoder derecho
#define ENC1_MOTD 3
#define ENC2_MOTD 12

// variables to store the number of encoder pulses

volatile long CountI = 0;
volatile long CountD = 0;

// Encoder motor izquierdo
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

// Encoder motor derecho
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
