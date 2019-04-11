#define PIN_TRIGGER_1 7
#define PIN_ECHO_2 A0
#define ESPERA_ENTRE_EMISION 3 // tiempo entre lecturas consecutivas en milisegundos

unsigned long cronometro = 0; // cronometro para emitir cada 3ms --> ESTO NO LO TENDREMOS
unsigned long reloj = 0; // reloj para calcular la media cada 10ms
float media = 0; // calcula la media de las mediciones
unsigned int n = 0; // cuenta el numero de mediciones

void setup()
{
  Serial.begin(9600);
  pinMode(PIN_TRIGGER_1, OUTPUT);
  pinMode(PIN_ECHO_2, INPUT);
  pinMode(13, OUTPUT);
  digitalWrite(PIN_TRIGGER_2, LOW);
  digitalWrite(13, LOW);
  delayMicroseconds(2);
}

void loop() {

  // EMITIENDO DE 1
  if (millis() - cronometro > ESPERA_ENTRE_EMISION) {
    digitalWrite(PIN_TRIGGER_1, HIGH); // Un pulso a nivel alto…
    delayMicroseconds(10); // …durante 10 µs y
    digitalWrite(PIN_TRIGGER_1, LOW); // …volver al nivel bajo
    cronometro = millis();
  }

  // LEE DE 2
  media += analogRead(PIN_ECHO_2);
  n++;
  if (millis() - reloj > 10) {
    media = media / n;
    Serial.println(media);
    if (media < 2.5) { // si la media es baja, tiene muchos 0 --> detecta pulso
      digitalWrite(13, HIGH); // enciende el LED
    } else { // si la media es alta, tiene pocos 0 --> no detecta pulso
      digitalWrite(13, LOW); // enciende el LED
    }
    media = 0;
    n = 0;
    reloj = millis();
  }
}