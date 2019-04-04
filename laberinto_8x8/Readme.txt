Explicación del programa del laberinto.

A grandes rasgos el programa lo que hace es moverse por el laberinto a derechas hasta llegar el centro.
Lo primero que hace es leer las paredes de la casilla en la que está (leer paredes) y guardar la posición de las paredes (actualizar paredes), por si en el futuro se implementa el mapeo.
Es muy importante tener en cuenta en todo momento la orientación del robot (ori) que permite orientarnos tanto en ejes globales (las casillas del laberinto, representado por hor y ver) como en ejes locales (los sensores que va a leer el robot y hacia donde se va a mover)
Una vez guardadas las paredes del laberinto decide hacia dónde se mueve (girar_derecha, girar_izquierda, mover_recto y dalavuelta_joder)
Tras recorrer la casilla actualiza su posición (hor y ver) y guarda la casilla en los vectores que representan el camino recorrido (camino_hor y camino_ver). El número de casillas recorridas es el contador j.
El recorrido termina cuando se llega a una de las cuatro casillas centrales ((hor == 3 && ver == 3) || (hor == 3 && ver == 4) || (hor == 4 && ver == 3) || (hor == 4 && ver == 4))
Cuando termino de recorrer tengo que eliminar los caminos redundantes, consiguiendo en el laberinto sin bucles el camino correcto. Lo hago en este caso es comprobar en todo el vector de casillas recorridas (camino_hor y camino_ver) si pasa dos veces por la misma casilla.
Si ocurre esto quiere decir que ha hecho un recorrido y ha vuelto, por lo que elimino desde la casilla que se repite con menor índice hasta la mayor (borrando también una de ellas para no repetir casilla).
Esto lo hago cambiando el valor de todas las casillas que quiero eliminar por 17 (un valor irreal) y una vez marcados todos los valores a eliminar se traspasan a un nuevo vector (camino_hor_def y camino_ver_def).
Una vez hecho esto se para en el centro (delay(2000)). La próxima vez que se ejecute el loop será conocido el camino. Para poder reiniciar el robot entre medias necesito guardar el camino en memoria (librería EEPROM), así como la bandera que me dice que ya he resuelto el laberinto (tengo el camino más corto posible) y la variable i, que me indica el número de casillas que tiene el camino (los elementos no nulos de camino_hor_def)
Si las variables están bien guardadas en memoria, la próxima vez que entre en el programa no entrará en "if (laberinto_resuelto != 1)" porque será 1 y entrará en el else, en el cual recupera de la memoria las variables que tenía y se recorre el laberinto conocido el camino.
El camino lo miro elemento a elemento (la casilla horizontal y la vertical), comparo con la casilla anterior y teniendo en cuenta lo que me tengo que mover en ejes globales (if (camino_hor_def [l+1] > camino_hor_def [l])) y la orientación en ese momento del robot me muevo hasta el centro.

Esta versión "mapea" (guarda las paredes del laberinto), pero no lo utiliza.

La única idea que no tengo muy clara es el valor de laberinto_resuelto.
La primera vez que lo utilizo es 0 y cuando lo resuelvo es 1, pero después de resolverlo a lo mejor quiero hacer dos o tres intentos seguidos de recorrerlo para mejorar el tiempo, por lo que no puedo volver a poner a 0 su valor al final del programa. Como lee este valor de la memoria, lo único que se me ocurre es que cuando queramos "mapear", cambiemos con otro programa la posición de memoria de la que lo estoy leyendo a 0.
