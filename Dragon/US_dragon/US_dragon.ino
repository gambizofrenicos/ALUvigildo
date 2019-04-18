#define PIN_TRIGGER_1 22
#define PIN_TRIGGER_2 7
#define PIN_ECHO_2 8
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

  pinMode(PIN_TRIGGER_2, OUTPUT);
  pinMode(PIN_ECHO_2, INPUT);

  pinMode(13, OUTPUT);
  digitalWrite(PIN_TRIGGER_1, LOW);
  digitalWrite(13, LOW);
  delayMicroseconds(2);
}

void loop() {


  // EMITIENDO DE 1

  digitalWrite(PIN_TRIGGER_1, HIGH); // Un pulso a nivel alto…


  // LEE DE 2
  /*media += analogRead(PIN_ECHO_2);
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
    Serial.println(promedio(200));
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
    }*/

  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  Serial.println(duration);

}

/*int promedio(int N)
  {
  long suma=0;
  for(int i=0;i<N;i++)
  {
    suma=suma+analogRead(A0);
  }
  return(suma/N);
  }*/
