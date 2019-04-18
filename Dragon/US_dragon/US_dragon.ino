#define PIN_TRIGGER_1 4
#define PIN_TRIGGER_2 8
#define PIN_ECHO_2 7
#define ESPERA_ENTRE_EMISION 3 // tiempo entre lecturas consecutivas en milisegundos

unsigned long cronometro = 0; // cronometro para emitir cada 3ms --> ESTO NO LO TENDREMOS
unsigned long reloj = 0; // reloj para calcular la media cada 10ms
float media = 5000; // calcula la media de las mediciones
unsigned int n = 1; // cuenta el numero de mediciones
long duration = 0;
float media_max = 0, media_min = 1023;

int t = 0;

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

  digitalWrite(PIN_TRIGGER_2, HIGH);
}

void loop() {


  // EMITIENDO DE 1
 digitalWrite(PIN_TRIGGER_1, LOW);
  delayMicroseconds(5);
  digitalWrite(PIN_TRIGGER_1, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIGGER_1, LOW);

  // LEE DE 2
  
  digitalWrite(PIN_TRIGGER_2, LOW);
  delayMicroseconds(5);
  digitalWrite(PIN_TRIGGER_2, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIGGER_2, LOW);

  pinMode(PIN_ECHO_2, INPUT);
  duration = pulseIn(PIN_ECHO_2, HIGH);
  if(duration != 42 && media > duration){
    media = duration;
  }
  Serial.print(media);
  Serial.print(" ");
  Serial.println(duration);
  
}
