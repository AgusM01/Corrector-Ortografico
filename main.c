#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "archivo.h"
#include "hash.h"
#include "glist.h"
#include "PalabraStruct.h"
#include "Reglas.h"
#include "Validas.h"

#define CANTIDAD_CHAR_INICIAL 10


int main(int argc, char *argv[]) {
  assert(argc == 4);
  /*
     El nombre de los archivos vienen dados por main siendo el primero el nombre del archivo a corregir 
     y el segundo el nombre del archivo de salida.
   */
  char *fentrada = strdup(argv[1]);
  char *fsalida = strdup(argv[2]);
  char *fdiccionario = strdup(argv[3]);

  int cantidad_tabla_hash = 100;        /*Cantidad inicial de elementos que se podran guardar en la tabla */

  /* 
   *   Crea la tabla hash la cual será la estructura utilizada para guardar el diccionario.
   *   La tabla guardará una estructura palabra la cual consiste en la palabra en cuestión y su longitud.
   */
  TablaHash diccionario =
      tablahash_crear(cantidad_tabla_hash,
                      (FuncionCopiadora) copia_palabra_struct,
                      (FuncionComparadora) compara_palabra_struct,
                      (FuncionHash) hash_palabra_struct,
                      (FuncionVisitante) imprime_palabra_struct,
                      (FuncionDatoDestructora) destruye_palabra_struct_final);

  /*
     Función encargada de leer el diccionario, guardando todas las palabras en la tabla hash-
   */
  lee_diccionario(diccionario, fdiccionario);

  /*Abre el archivo de entrada para lectura */
  FILE *f_entrada;
  f_entrada = fopen(fentrada, "r");
  assert(f_entrada != NULL);

  /* 
     En esta tabla se guardarán las palabras ya revisadas.
     Cada elemento de esta tabla es una estructura de tipo "Valida". 
     Esta estructura contiene_
     *Palabra invalida (char*)
     *Longitud de la palabra invalida (int)
     *Cantidad de sugerencias (int)
     *Sugerencias encontradas (char**)
     Esta tabla sirve para el caso en que en el archivo de entrada aparezca 2 veces una misma palabra.
     En este caso no tendremos que corregir nuevamente la palabra ya que habremos guardado las sugerencias 
     encontradas.
     Además sirve para la nueva implementación,
   */
  TablaHash tabla_validas =
      tablahash_crear(cantidad_tabla_hash, (FuncionCopiadora) copia_validas,
                      (FuncionComparadora) compara_validas,
                      (FuncionHash) hash_validas,
                      (FuncionVisitante) imprime_validas,
                      (FuncionDatoDestructora) destruye_validas);

  /*
     Abre el archivo cache.
     En este archivo vendran palabras corregidas en textos anteriores y sugerencias encontradas 
     para las mismas.
     En el caso de tener que corregir un texto de entrada que tiene palabras corregidas en textos pasados,
     no deberemos corregir nuevamente la palabra (aplicar todas las reglas), si no que 
     podremos buscar directamente esta palabra y las sugerencias encontradas en la tabla hash de Validas.

   */
  FILE *f_cache;
  f_cache = fopen("cache.txt", "a+");
  assert(f_cache != NULL);

  /*
     Lee el archivo cache tomando las palabras, cantidad de sugerencias y sugerencias que 
     aparecen en el mismo, creando una estructura tipo "Validas" con esta informacion
     y guardando esta estructura en la tabla hash de "Validas" (tabla_validas).
   */
  lee_cache(f_cache, tabla_validas);

  /*Abre el archivo de salida */
  FILE *f_salida;
  f_salida = fopen(fsalida, "w");
  assert(f_salida != NULL);

  /*
     Con toda la informacion recopilada (diccionario y palabras ya revisadas),
     se encarga de leer el tecto de entrada y verificar que cada palabra esté en el 
     diccionario. 
     De lo contrario, a la palabra que no esté le aplicará cada una de las reglas de correción
     generando nuevas palabras y checkeando si cada una está en el diccionario.
     Si llega a 5 sugerencias, la palabra ya se encontraria "corregida".
     Si no se encuentran 5 sugerencias, se raliza otra aplicacion de las reglas sobre cada una 
     de las palabras generadas previamente.
     Este paso se hace hasta un maximo de 3 veces. Si no encontró 5 sugerencias,
     imprime las encontradas.
   */
  escribe_salida(f_entrada, f_cache, f_salida, diccionario, tabla_validas);

  /*
     Cierra y libera memoria,
   */
  free(fentrada);
  free(fsalida);
  free(fdiccionario);
  fclose(f_entrada);
  fclose(f_salida);
  fclose(f_cache);
  tablahash_destruir(tabla_validas);
  tablahash_destruir(diccionario);
  return 0;
}
