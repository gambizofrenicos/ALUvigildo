#define PIN_TRIGGER_1 8
#define PIN_ECHO_2 A0
#define ESPERA_ENTRE_EMISION 3 // tiempo entre lecturas consecutivas en milisegundos

unsigned long cronometro = 0; // cronometro para emitir cada 3ms --> ESTO NO LO TENDREMOS
unsigned long reloj = 0; // reloj para calcular la media cada 10ms
float media = 0; // calcula la media de las mediciones
unsigned int n = 0; // cuenta el numero de mediciones
long duration = 0;
float media_max = 0, media_min = 1023;
void setup()
{
  Serial.begin(9600);
  pinMode(PIN_TRIGGER_1, OUTPUT);
  pinMode(PIN_ECHO_2, INPUT);
  pinMode(13, OUTPUT);
  digitalWrite(PIN_TRIGGER_1, LOW);
  digitalWrite(13, LOW);
  delayMicroseconds(2);
}

void loop() {
<<<<<<< HEAD

  // EMITIENDO DE 1
  /* if (millis() - cronometro > ESPERA_ENTRE_EMISION) {
     for (int i = 0; i < 3; i++) {
       digitalWrite(PIN_TRIGGER_1, HIGH); // Un pulso a nivel alto…
       delayMicroseconds(10); // …durante 10 µs y
       digitalWrite(PIN_TRIGGER_1, LOW);
       delayMicroseconds(10); // …volver al nivel bajo
     }
     cronometro = millis();
    }*/
  digitalWrite(PIN_TRIGGER_1, HIGH); // Un pulso a nivel alto…
  delayMicroseconds(10); // …durante 10 µs y
  digitalWrite(PIN_TRIGGER_1, LOW);
  delayMicroseconds(30); // …volver al nivel bajo

  // LEE DE 2
  /*  media += analogRead(PIN_ECHO_2);
    n++;
    if (millis() - reloj > 10) {
      media = media / n;
      Serial.println(analogRead(PIN_ECHO_2));
      if (media < 2.5) { // si la media es baja, tiene muchos 0 --> detecta pulso
        digitalWrite(13, HIGH); // enciende el LED
      } else { // si la media es alta, tiene pocos 0 --> no detecta pulso
        digitalWrite(13, LOW); // enciende el LED
      }*/

  /*  Serial.print("media:");
     Serial.print("\t");
     Serial.print(media);
     Serial.print("\t\t");
     Serial.print("echo:");
     Serial.print("\t");
     Serial.println(analogRead(PIN_ECHO_2));*/

  /*  media = 0;
    n = 0;
    reloj = millis();*/
  //}


=======
  /*
    // EMITIENDO DE 1
    if (millis() - cronometro > ESPERA_ENTRE_EMISION) {
      digitalWrite(PIN_TRIGGER_1, HIGH); // Un pulso a nivel alto…
      delayMicroseconds(10); // …durante 10 µs y
      digitalWrite(PIN_TRIGGER_1, LOW); // …volver al nivel bajo
      cronometro = millis();
    }
  */

  // LEE DE 2
  media += analogRead(PIN_ECHO_2);
  n++;

  if (millis() - reloj > 10) {
    media = media / n;
    if (media > media_max) {
      media_max = media;
    } else if (media < media_min) {
      media_min = media;
    }
    Serial.print(media);
    Serial.print(" ");
    Serial.print(media_max);
    Serial.print(" ");
    Serial.println(media_min);
    if (media < (media_max + media_min) / 2) { // si la media es baja, tiene muchos 0 --> detecta pulso
      digitalWrite(13, HIGH); // enciende el LED
    } else { // si la media es alta, tiene pocos 0 --> no detecta pulso
      digitalWrite(13, LOW); // enciende el LED
    }
    media = 0;
    n = 0;
    reloj = millis();
  }
  if (millis() - cronometro > 5000) {
    media_max = 0;
   // media_min = 1023;
    cronometro = millis();
  }
>>>>>>> c494357e7e5044c74f22a1066466b1aa726c2c69
}
