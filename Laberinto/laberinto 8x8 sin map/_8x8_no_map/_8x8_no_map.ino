#include <Math.h>
#include <EEPROM.h>
#include "encoder.h"
#include "pid.h"
#include "motores.h"


#define SHARPI A0
#define SHARPA 10
#define SHARPO A1

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
void leer_paredes(){
  dist_i = 2076.0 / (analogRead(SHARPI) - 11.0);
  dist_d = 2076.0 / (analogRead(SHARPO) - 11.0);
  if (dist_i < 15){
    paredes_sensor[0] = 1;
    }
  if (dist_d < 15){ 
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
  if (laberinto_resuelto != 1){ //pongo distinto de 1 en vez de igual a 0 porque así la primera vez que ejecuto el programa y leo de la memoria es más probable que sea distinto de 1 a que sea 0
    if ((hor == 3 && ver == 3) || (hor == 3 && ver == 4) || (hor == 4 && ver == 3) || (hor == 4 && ver == 4)){ //lo he estadi resolviendo y he llegado al centro
      laberinto_resuelto = 1; //ojalá fuera tan fácil
      for (k=1; k++; k<j){ //la primera posición no la voy a quitar
        for (l=k-1; l--; l>=0){
          if ((camino_hor[k]==camino_hor[l])&&(camino_ver[k]==camino_ver[l])){ //"elimino" los caminos repetidos, cuando una casilla coincide, borro todas las casillas entre ellos
            for(i=l; i++; i<k){
              camino_hor[i] = 17;
              camino_ver[i] = 17; //pensaba darle el valor -1 pero el caso es ponerle un valor que no sea real
              }
            }
          }
        }
        i=0;
        for (k=0; k++; k<j){
         if (camino_hor[k] != 17){
          camino_hor_def[i] = camino_hor[k]+1;
          camino_ver_def[i] = camino_ver[k]+1; //esto lo hago para que el vector con el camino tenga las casillas entre 1x1 y 8x8 en lugar de ser en función de los índices. así solo guardo los elementos del vector que no son cero (no guardo los elementos que quedan vacíos después)
          i++;
         } //solo guardo los que no son repetidos
        }
        for (l=0; l++; l<i){ //guardo en la memoria cada uno de los elementos, porque son todos de un byte
          EEPROM.write (l, camino_hor_def[l]);
          EEPROM.write (999-l, camino_ver_def[l]); //cada una va en uno de los extremos de la memoria, para evitar sobreescribir
          }
        EEPROM.write (499, i); //me interesa guardar este contador, que como se ve 7-10 líneas más arriba tiene el número de componentes del vector que guarda el camino, para ver cuántas posiciones de memoria tengo que leer
        EEPROM.write (500, laberinto_resuelto); //esta también la quiero guardar para la próxima vez que entre saber que está resuelto y no volver a mapear
        delay (2000); //lo dejo parado en el centro. Cuando acabe este tiempo se moverá como si estuviera el principio aunque no lo toquemos pero me da igual, lo importante es guardar el camino y que lo he resuelto, al reiniciar solo quedará eso



        //Hasta ahora he descrito lo que pasa si estaba en modo "resolver laberinto" y llego al centro, es decir, el primer intento


        
      } else { //lo que hago si aún no he resuelto el laberinto y aún no estoy en el centro
      // voy a ir siempre a la derecha por arriba España y eso
        leer_paredes();
        if (paredes_sensor[2] == 0){
          girar90D();
          avanza_mm(150);
          switch (ori) {
            case 1:
              ori = 2;
              hor = hor + 1;
              break;
            case 2:
              ori = 3;
              ver = ver - 1;
              break;
            case 0:
              ori = 1;
              ver = ver + 1;
              break;
            case 3:
              ori = 0;
              hor = hor - 1;
            }
            pos_act [0] = hor;
            pos_act [1] = ver;
            j++;
            camino_hor [j] = hor;
            camino_ver [j] = ver;
          } else {
            if (paredes_sensor[1] == 0){
              avanza_mm(168);
              switch (ori) {
                case 1:
                  ver = ver + 1;
                  break;
                case 2:
                  hor = hor + 1;
                  break;
                case 0:
                  hor = hor - 1;
                  break;
                case 3:
                  ver = ver - 1;
              }
             } else {
               if (paredes_sensor[0] == 0){
                girar90I(); //ojo, tanto mover izquierda como mover derecha se refieren a ir de una casilla a la inmediatamente contigua a ese lado, como si tuviéramos omniruedas. Esto quiere decir que la cantidad de tiempo que me muevo recto es distinto que cuando sigo recto ya que tengo que restar el giro
                avanza_mm(150);
                switch (ori) {
                  case 1:
                    ori = 0;
                    hor = hor - 1;
                    break;
                  case 2:
                    ori = 1;
                    ver = ver + 1;
                    break;
                  case 0:
                    ori = 3;
                    ver = ver - 1;
                    break;
                  case 3:
                    ori = 2;
                    hor = hor + 1;
                  }
                } else {
                  girar180(); //para unificar el código lo que quiero es dar la vuelta y mover una casilla recto (no va a haber pared porque solo hago esto si hay pared en las otras 3 direcciones)
                  avanza_mm(150);
                  switch (ori) {
                  case 1:
                    ori = 3;
                    ver = ver - 1;
                    break;
                  case 2:
                    ori = 0;
                    hor = hor - 1;
                    break;
                  case 0:
                    ori = 2;
                    hor = hor + 1;
                    break;
                  case 3:
                    ori = 1;
                    ver = ver + 1;
                  }
                  }
                }
              }
        pos_act [0] = hor;
        pos_act [1] = ver; //actualizo mi nueva posición
        j++;
        camino_hor [j] = hor; //guardo el movimiento en el caminito
        camino_ver [j] = ver;
          }


          //todo esto es el caso en el que estoy navegando por el laberinto (sin saber el camino) y aún no he llegado al centro


          
  } else { //lo que hago si SÍ he resuelto el laberinto
    i = EEPROM.read (499); 
    for (l=0; l++; l<i){
      camino_hor_def [l] = EEPROM.read (l); 
      camino_ver_def [l] = EEPROM.read (999-l);
      }
    for (l=1; l++; l<i){
      if (camino_hor_def [l+1] > camino_hor_def [l]){ //el camino me dice que debo moverme una casilla en horizontal (hacia la derecha)
        switch (ori) {
          case 1:
          girar90D();
          avanza_mm(150);
          break;
          case 2:
          avanza_mm(168);
          break;
          case 0: //este caso no va a pasar, muy raro sería que una vez mapeado tenga que darme la vuelta
          girar180();
          avanza_mm(150);
          break;
          case 3:
          girar90I();
          avanza_mm(150);
          break;
          }
          ori = 2; //si me muevo una casilla hacia la derecha, mi orientación al final será hacia la derecha
        } else {
          if (camino_ver_def[l+1] > camino_ver_def[l]){ //tengo que moverme una casilla en vertical (hacia delante)
              switch (ori) {
              case 1:
              avanza_mm(168);
              break;
              case 2:
              girar90I();
              avanza_mm(150);
              break;
              case 0: 
              girar90D();
              avanza_mm(150);
              break;
              case 3: //este caso no va a pasar, muy raro sería que una vez mapeado tenga que darme la vuelta
              girar180();
              avanza_mm(150);
              break;
              }
            ori = 1; //si me muevo una casilla hacia delante, mi orientación al final será hacia delante
            } else {
              if (camino_hor_def[l+1] < camino_hor_def[l]){ //tengo que moverme una casilla en horizontal (hacia la izquierda)
                switch (ori) {
                  case 1:
                  girar90I();
                  avanza_mm(150);
                  break;
                  case 2: //este caso no va a pasar, muy raro sería que una vez mapeado tenga que darme la vuelta
                  girar180();
                  avanza_mm(150);
                  break;
                  case 0: 
                  avanza_mm(168);
                  break;
                  case 3:
                  girar90D();
                  avanza_mm(150); 
                  break;
                }
            ori = 0; //si me muevo una casilla hacia la izquierda, mi orientación al final será hacia la izquierda
            } else { //el último caso es que me tenga que mover hacia atrás (una casilla en vertical)
                switch (ori) {
                  case 1: //este caso no va a pasar, muy raro sería que una vez mapeado tenga que darme la vuelta
                  girar180();
                  avanza_mm(150);
                  break;
                  case 2: 
                  girar90D();
                  avanza_mm(150);
                  break;
                  case 0:
                  girar90I(); 
                  avanza_mm(150);
                  break;
                  case 3:
                  avanza_mm(150);
                  break;
                }
            ori = 3; //si me muevo una casilla hacia la izquierda, mi orientación al final será hacia la izquierda
              }
            }
            }
          }
   delay (2000); 
 }



      //el último fragmento es la navegación por el laberinto una vez conocido el camino (el segundo intento y sucesivos)


      
}
//laberinto_resuelto puedo cambiarlo de 0 a 1 pero, ¿cuándo lo vuelvo a poner a 0? En teoría deberé modificar la memoria por otro lado para ponerle un 0 cuando quiera mapear en vez de resolver
