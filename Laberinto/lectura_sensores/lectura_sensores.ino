#define SHARPI A0
#define SHARPA 10
#define SHARPO A1

void setup() {
  // put your setup code here, to run once:
pinMode(SHARPI, INPUT);
pinMode(SHARPA, INPUT);
pinMode(SHARPA, INPUT);
Serial.begin(9600);
}

float dist_i;
float dist_d;
int pared_a;

void loop() {
  // put your main code here, to run repeatedly:
  dist_i=analogRead(SHARPI);
  dist_d=analogRead(SHARPO);
  pared_a=digitalRead(SHARPA);
  Serial.println("----------");
  Serial.println("Alante:");
  Serial.println(pared_a);
  Serial.println("Derecha:");
  Serial.println(dist_d);
  Serial.println("Izquierda:");
  Serial.println(dist_i);

  delay(500);
}
