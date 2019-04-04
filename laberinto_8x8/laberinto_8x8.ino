#include <EEPROM.h>

typedef struct casilla {
  boolean paredes [4]; // 0 si no hay pared en cada casilla y 1 si hay. El orden es izquierda, delante, derecha y detrás
  boolean caminos [4]; // 0 si no hay camino/ya ha sido visitado y 1 si aún no ha sido visitado. El orden es izquierda, delante, derecha y detrás
  boolean completo; // 1 si conozco todas las paredes, consideraría esa casilla como visitada
  } casilla;


struct casilla laberinto [8][8]; // la idea es pasar por todas las casillas y guardar las paredes que tiene cada una de ellas, el problema es pasar por todos ellos. ¿Basta dividir en caminos y saber por los que he pasado y por los que no?
uint8_t hor = 0;
uint8_t ver = 0;
uint8_t pos_act [2] = {0, 0};
uint8_t pos_pre [2] = {0, 0};
uint8_t camino_hor [150]; //podría ajustar incluso más, no creo que pase por 500 casillas en el primer intento
uint8_t camino_ver [150];
uint8_t camino_hor_def [60]; //también puedo ajustar más, pero como no repito casillas como máximo tendré que recorrerlas todas
uint8_t camino_ver_def [60];
uint8_t last_camino [2]; // Es la última casilla en la cual he dejado un camino sin visitar, a la que voy cuando termino el camino actual
uint8_t ori; //0 si girado hacia la izquierda, 2 si girado hacia la derecha, 1 si va hacia adelante y 3 si va hacia atrás
boolean paredes_sensor [4]; //leídas con los sensores, es decir, desde el sistema de referencia del robot. La pared de atrás en teoría la conozco del movimiento anterior.
uint8_t i = 0; //siempre te hace falta un buen índice
uint8_t j = 0; //este es para añadir nuevos valores a los arrays que guardan el camino
uint8_t k = 0;
uint8_t l = 0;
boolean laberinto_resuelto = 0; //esta deberé leer en memoria ¿en el setup? para actualizarla por si la he guardado en un intento anterior y sí he resuelto el laberinto
                                //si leo y guardo en memoria igual un boolean no es lo mejor
void leer_paredes(){
  //movidas de leer con los sensores, hay que tener especial cuidado con leer la pared de enfrente ya que el sensor es digital
  //ahora lo que hago es definir la posición trasera a partir de la anterior conocida. La única excepción es (creo) el primer caso, en el que no tengo posición anterior
  if ((hor == 0)&&(ver == 0)){
    paredes_sensor[3] = 0;
  } else {
    switch (ori){
      case 1: paredes_sensor[3] = laberinto[hor][ver - 1].paredes[1]; break;
      case 2: paredes_sensor[3] = laberinto[hor - 1][ver].paredes[2]; break;
      case 0: paredes_sensor[3] = laberinto[hor + 1][ver].paredes[0]; break;
      case 3: paredes_sensor[3] = laberinto[hor][ver + 1].paredes[3]; break;
      }
    }
  }

void actualizar_paredes(){
  switch (ori){
   case 1:
    laberinto[hor][ver].paredes[0] = paredes_sensor[0];
    laberinto[hor][ver].paredes[1] = paredes_sensor[1];
    laberinto[hor][ver].paredes[2] = paredes_sensor[2];
    laberinto[hor][ver].paredes[3] = paredes_sensor[3];
    break;
   case 2:
    laberinto[hor][ver].paredes[0] = paredes_sensor[3];
    laberinto[hor][ver].paredes[1] = paredes_sensor[0];
    laberinto[hor][ver].paredes[2] = paredes_sensor[1];
    laberinto[hor][ver].paredes[3] = paredes_sensor[2];
    break;
   case 0: 
    laberinto[hor][ver].paredes[0] = paredes_sensor[1];
    laberinto[hor][ver].paredes[1] = paredes_sensor[2];
    laberinto[hor][ver].paredes[2] = paredes_sensor[3];
    laberinto[hor][ver].paredes[3] = paredes_sensor[0];
    break;
   case 3:
    laberinto[hor][ver].paredes[0] = paredes_sensor[2];
    laberinto[hor][ver].paredes[1] = paredes_sensor[3];
    laberinto[hor][ver].paredes[2] = paredes_sensor[0];
    laberinto[hor][ver].paredes[3] = paredes_sensor[1]; 
    break;
   }
  laberinto[hor][ver].completo = 1;
  }
  void girar_derecha(){
    
    }
  void girar_izquierda(){
    
    }
  void mover_recto(){
    
    }
  void dalavuelta_joder(){
    
    }
  void setup() {
  //movidas de pines, loco
  ori = 1;
  camino_hor [j] = 0;
  camino_ver [j] = 0;
  laberinto[3][3].paredes[1] = 0;
  laberinto[3][3].paredes[2] = 0;
  laberinto[3][4].paredes[2] = 0;
  laberinto[3][4].paredes[3] = 0;
  laberinto[4][3].paredes[0] = 0;
  laberinto[4][3].paredes[1] = 0;
  laberinto[4][4].paredes[0] = 0;
  laberinto[4][4].paredes[3] = 0; //es un poco overkill, son paredes compartidas entre varias casillas, igual puede definirse de otra manera, pero lo vería muy lioso
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
        if (laberinto[hor][ver].completo != 1){ //no leo de nuevo si ya conozco la casilla
          leer_paredes();
          actualizar_paredes();
      }
        if (laberinto[hor][ver].paredes[2] == 0){
          girar_derecha();
          mover_recto();
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
            if (laberinto[hor][ver].paredes[1] == 0){
              mover_recto();
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
               if (laberinto[hor][ver].paredes[0] == 0){
                girar_izquierda(); //ojo, tanto mover izquierda como mover derecha se refieren a ir de una casilla a la inmediatamente contigua a ese lado, como si tuviéramos omniruedas. Esto quiere decir que la cantidad de tiempo que me muevo recto es distinto que cuando sigo recto ya que tengo que restar el giro
                mover_recto();
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
                  dalavuelta_joder(); //para unificar el código lo que quiero es dar la vuelta y mover una casilla recto (no va a haber pared porque solo hago esto si hay pared en las otras 3 direcciones)
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
        for (l=0; l++; l<2){
         pos_pre[l] = pos_act[l]; 
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
          girar_derecha();
          mover_recto();
          break;
          case 2:
          mover_recto();
          break;
          case 0: //este caso no va a pasar, muy raro sería que una vez mapeado tenga que darme la vuelta
          dalavuelta_joder();
          break;
          case 3:
          girar_izquierda();
          mover_recto();
          break;
          }
          ori = 2; //si me muevo una casilla hacia la derecha, mi orientación al final será hacia la derecha
        } else {
          if (camino_ver_def[l+1] > camino_ver_def[l]){ //tengo que moverme una casilla en vertical (hacia delante)
              switch (ori) {
              case 1:
              mover_recto();
              break;
              case 2:
              girar_izquierda();
              mover_recto();
              break;
              case 0: 
              girar_derecha();
              mover_recto();
              break;
              case 3: //este caso no va a pasar, muy raro sería que una vez mapeado tenga que darme la vuelta
              dalavuelta_joder();
              break;
              }
            ori = 1; //si me muevo una casilla hacia delante, mi orientación al final será hacia delante
            } else {
              if (camino_hor_def[l+1] < camino_hor_def[l]){ //tengo que moverme una casilla en horizontal (hacia la izquierda)
                switch (ori) {
                  case 1:
                  girar_izquierda();
                  mover_recto();
                  break;
                  case 2: //este caso no va a pasar, muy raro sería que una vez mapeado tenga que darme la vuelta
                  dalavuelta_joder();
                  break;
                  case 0: 
                  mover_recto();
                  break;
                  case 3:
                  girar_derecha();
                  mover_recto();
                  break;
                }
            ori = 0; //si me muevo una casilla hacia la izquierda, mi orientación al final será hacia la izquierda
            } else { //el último caso es que me tenga que mover hacia atrás (una casilla en vertical)
                switch (ori) {
                  case 1: //este caso no va a pasar, muy raro sería que una vez mapeado tenga que darme la vuelta
                  dalavuelta_joder();
                  break;
                  case 2: 
                  girar_derecha();
                  mover_recto();
                  break;
                  case 0:
                  girar_izquierda(); 
                  mover_recto();
                  break;
                  case 3:
                  mover_recto();
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
