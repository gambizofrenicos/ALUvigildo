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
  dist_i= 2076.0/(analogRead(SHARPI)-11.0);
  dist_d=2076.0/(analogRead(SHARPO)-11.0  );
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
