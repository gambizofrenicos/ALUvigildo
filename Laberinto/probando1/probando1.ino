#define SHARPI A0
#define SHARPA 10
#define SHARPO A1

#include <Math.h>
#include <EEPROM.h>
#include "encoder.h"
#include "pid.h"
#include "motores.h"

/*Laberinto*/
uint8_t hor = 0;
uint8_t ver = 0;
uint8_t pos_act [2] = {0, 0}; //Si funciona bien, esta podría incluso quitarla
uint8_t camino_hor [150];
uint8_t camino_ver [150];
uint8_t camino_hor_def [60]; //también puedo ajustar más, pero como no repito casillas como máximo tendré que recorrerlas todas
uint8_t camino_ver_def [60];
uint8_t ori; //0 si girado hacia la izquierda, 2 si girado hacia la derecha, 1 si va hacia adelante y 3 si va hacia atrás
boolean paredes_sensor [3]; //leídas con los sensores, es decir, desde el sistema de referencia del robot. Paso de la pared de atrás.
float dist_i;
float dist_d;
uint8_t i = 0; //siempre te hace falta un buen índice
uint8_t j = 0; //este es para añadir nuevos valores a los arrays que guardan el camino
uint8_t k = 0;
uint8_t l = 0;
boolean laberinto_resuelto = 0; //esta deberé leer en memoria ¿en el setup? para actualizarla por si la he guardado en un intento anterior y sí he resuelto el laberinto
//si leo y guardo en memoria igual un boolean no es lo mejor
void leer_paredes() {
  dist_i = 2076.0 / (analogRead(SHARPI) - 11.0);
  dist_d = 2076.0 / (analogRead(SHARPO) - 11.0);
  if (dist_i < 15) {
    paredes_sensor[0] = 1;
  }
  if (dist_d < 15) {
    paredes_sensor[2] = 1;
  }
  paredes_sensor[1] = !(digitalRead(SHARPA));
}

void setup() {
  /* Serial */
  Serial.begin(9600);

  /* Motores */
  pinMode(DIRD, OUTPUT);
  pinMode(DIRI, OUTPUT);
  pinMode(PWMD, OUTPUT);
  pinMode(PWMI, OUTPUT);
  pinMode(ENC1_MOTI, INPUT);
  pinMode(ENC2_MOTI, INPUT);
  pinMode(ENC1_MOTD, INPUT);
  pinMode(ENC2_MOTD, INPUT);
  pinMode(SHARPI, INPUT);
  pinMode(SHARPO, INPUT);
  pinMode(SHARPA, INPUT);

  /*Interrupción encoder*/
  attachInterrupt(0, EncoderEventMotI, CHANGE);
  attachInterrupt(1, EncoderEventMotD, CHANGE);

  /*Laberinto*/
  ori = 1;
  camino_hor [j] = 0;
  camino_ver [j] = 0;
  laberinto_resuelto = EEPROM.read (500); // necesito leerla aquí. Puede ser un problema la primera vez que ejecuto, porque valdrá lo que haya en la memoria. Si por casualidad vale 1 se va a creer que ya he mapeado cuando no. Es posible que tenga que ponerla a 0 con otro programa.
}

void loop() {
  leer_paredes();
  if (paredes_sensor[2] == 0) {
    girar90D();
    para();
    delay(500);
    avanza_mm_lab(50);
    // para();
    // delay(2000);
  } else {
    if (paredes_sensor[1] == 0) {

      error((2076.0 / (analogRead(SHARPI) - 11.0)), (2076.0 / (analogRead(SHARPO) - 11.0)));
      pwmi = PWM - PID_lab;
      pwmd = PWM + PID_lab;
      acotar();
      avanzar();
      para();
      delay(2000);
    } else {
      if (paredes_sensor[0] == 0) {
        girar90I(); //ojo, tanto mover izquierda como mover derecha se refieren a ir de una casilla a la inmediatamente contigua a ese lado, como si tuviéramos omniruedas. Esto quiere decir que la cantidad de tiempo que me muevo recto es distinto que cuando sigo recto ya que tengo que restar el giro
        para();
        delay(500);
        avanza_mm_lab(150);
        para();
        delay(2000);
      } else {
        if (paredes_sensor[1] == 1) {
          girar90D();
          girar90D();
          para();
          delay(500);
          avanza_mm_lab(140);
          para();
          delay(2000);
        }
      }
    }
  }
  // j++;
  //  Serial.println (j);
}
