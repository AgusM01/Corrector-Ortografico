#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Reglas.h"
#include "utils.h"
#include "Validas.h"
#include "glist.h"
#include "PalabraStruct.h"
#include "hash.h"

/*
 *Crea una estructura validas.
*/
Validas crea_validas(char *pal_invalida, int pal_invalida_length) {
  Validas estructura_validas = malloc(sizeof(struct _Validas));
  estructura_validas->pal_invalida = strdup(pal_invalida);
  estructura_validas->cant = 0;
  estructura_validas->pal_invalida_length = pal_invalida_length;
  for (int i = 0; i < 5; i++)
    estructura_validas->lista_sugerencias[i] = NULL;
  return estructura_validas;
}

/*
 *Destruye una estructura validas.
*/
void destruye_validas(Validas estructura_validas) {
  free(estructura_validas->pal_invalida);
  for (int i = 0; i < estructura_validas->cant; i++)
    free(estructura_validas->lista_sugerencias[i]);
  free(estructura_validas);
  return;
}

/*
 *Agrega una sugerencia de la palabra a su estructura.
*/
Validas agrega_sugerencia_validas(Validas estructura_validas, char *sugerencia) {
  estructura_validas->lista_sugerencias[estructura_validas->cant] =
      strdup(sugerencia);
  estructura_validas->cant++;
  return estructura_validas;
}

/*
 *Compara si dos estructuras validas son iguales en base a su palabra invalida.
 *Devuelve 0 si son iguales, otro entero caso contrario (implementacion de strcmp).
*/
int compara_validas(Validas estructura_validas1, Validas estructura_validas2) {
  return strcmp(estructura_validas1->pal_invalida,
                estructura_validas2->pal_invalida);
}

/*
 *Copia una estructura validas.
*/
Validas copia_validas(Validas estructura_validas) {
  Validas estructura_copia =
      crea_validas(estructura_validas->pal_invalida,
                   estructura_validas->pal_invalida_length);
  estructura_copia->cant = estructura_validas->cant;
  for (int i = 0; i < estructura_validas->cant; i++)
    estructura_copia->lista_sugerencias[i] =
        strdup(estructura_validas->lista_sugerencias[i]);
  return estructura_copia;
}

/*
 *Calcula el indice de hash de una estructura validas.
*/
unsigned hash_validas(Validas estructura_validas) {
  return djb2(estructura_validas->pal_invalida);
}

/*
 *Imprime una estrucura validas.
*/
void imprime_validas(Validas estructura_validas) {
  printf("Palabra invalida: %s\n", estructura_validas->pal_invalida);
  printf("\t\t\tCantidad de sugerencias: %d\n", estructura_validas->cant);
  printf("\t\t\tLista de sugerencias:\n");
  for (int i = 0; i < estructura_validas->cant; i++)
    printf("\t\t\t%s\n", estructura_validas->lista_sugerencias[i]);
  return;
}

/*
 *Funcion la cual se encargara de tomar una palabra y corregirla,
 *aplicando las reglas de correccion sobre la misma y sus generaciones en caso de no 
 *encontrar 5 sugerencias.
 *Esto se hace hasta una distancia de "generacion" de 3.
 *Si luego de analizar la 3ra generacion no se encuentran 5 sugerencias,
 *se muestran las que se encontraron.
*/
Validas corregir(Validas palabra_valida, PalabraStruct palabra,
                 TablaHash diccionario) {
  /*Crea una tabla hash donde se guardaran las palabras ya revisadas para no tener que revisarlas nuevamente. */
  TablaHash tabla_revisadas =
      tablahash_crear(100, (FuncionCopiadora) copia_palabra_struct,
                      (FuncionComparadora) compara_palabra_struct,
                      (FuncionHash) hash_palabra_struct,
                      (FuncionVisitante) imprime_palabra_struct,
                      (FuncionDatoDestructora) destruye_palabra_struct_final);

  /*Esta lista enlazada representara la generacion a analizar. */
  GPunteros distancia_1 = glist_crear_punteros();

  /*Agrega la palabra a la lista ya que esta seria la generacion "cero" */
  distancia_1 =
      glist_agregar_final(distancia_1, palabra,
                          (FuncionCopiaGlist) copia_palabra_struct);
  /* Variable entera que indica en que generacion estoy iterando, sirve para pasar como bandera indicando a checkeo
     que si ya estoy en la tercer distancia, que no agregue esas palabras a otra nueva lista ya que no se checkearan.
   */
  int store_words = 1;

  /* 3 iteraciones, una por cada distancia */
  for (int i = 0; i <= 2 && palabra_valida->cant < 5; i++) {
    /*Lista enlazada en la que guardaré las palabras de la siguiente genracion. */
    GPunteros distancia_2 = glist_crear_punteros();
    /*Si estoy en la ultima distancia, pongo store_words en cero */
    if (i == 2)
      store_words = 0;
    /* Voy iterando sobre los elementos de mi lista las cuales seran las palabras a aplicarles reglas para poder
       obtener sugerencias.
     */
    for (GList temp = distancia_1->primero;
         temp != NULL && palabra_valida->cant < 5; temp = temp->next) {
      palabra_valida =
          intercambiar(diccionario, palabra_valida, tabla_revisadas,
                       distancia_2, temp->data, store_words);
      palabra_valida =
          insertar(diccionario, palabra_valida, tabla_revisadas, distancia_2,
                   temp->data, store_words);
      palabra_valida =
          eliminar(diccionario, palabra_valida, tabla_revisadas, distancia_2,
                   temp->data, store_words);
      palabra_valida =
          reemplazar(diccionario, palabra_valida, tabla_revisadas, distancia_2,
                     temp->data, store_words);
      palabra_valida = separar(diccionario, palabra_valida, temp->data);

    }
    /*Destruyo la lista sobre la que estaba iterando */
    glist_destruir(distancia_1,
                   (FuncionDestructoraGlist) destruye_palabra_struct_final);
    /*Cambio los punteros de la lista indicando que quiero iterar sobre la proxima generacion */
    distancia_1 = distancia_2;
  }
  /*Destruyo la ultima lista. */
  glist_destruir(distancia_1,
                 (FuncionDestructoraGlist) destruye_palabra_struct_final);
  /*Destruyo la tabla hash */
  tablahash_destruir(tabla_revisadas);
  return palabra_valida;
}

/*
 *Checkea si una palaba generada mediante una de las reglas es valida para que 
 *sea una sugerencia (está en el diccionario).
 *Además, en el caso de no estar trabajando en la 2da generacion de sucesores,
 *agrega las palabras generadas a la lista de los proximos sucesores a analizar.
*/
Validas checkeo(Validas estructura_validas, TablaHash tabla,
                TablaHash tabla_revisadas, GPunteros lista_invalidas,
                PalabraStruct newPalabra_struct, int store_words) {

  /*Si ya tengo 5 sugerencias, retorno */
  if (estructura_validas->cant == 5)
    return estructura_validas;
  /*Si ya revise la palabra, retorno */
  if (tablahash_buscar(tabla_revisadas, newPalabra_struct) != NULL)
    return estructura_validas;

  int flag = 1, compara = 0;

  /*Checkeo que la palabra que estoy verificando no sea una sugerencia ya puesta */
  for (int i = 0; i < estructura_validas->cant; i++) {
    compara =
        strcmp(newPalabra_struct->palabra,
               estructura_validas->lista_sugerencias[i]);
    if (compara == 0)
      flag = 0;

  }

  if (flag) {
    /*Si no es una sugerencia previa, busco la palabra en el diccionario */
    if (tablahash_buscar(tabla, newPalabra_struct) != NULL) {
      /*Si está, significa que es una sugerencia valida, por lo que la agrego a la lista de sugerencias
         de la palabra original (la que está en el archivo de entrada) */
      estructura_validas =
          agrega_sugerencia_validas(estructura_validas,
                                    newPalabra_struct->palabra);
    }
    if (store_words) {
      /*Si no estoy en distancia 3, guardo la palabra para poder aplicarle las reglas. */
      lista_invalidas =
          glist_agregar_final(lista_invalidas, newPalabra_struct,
                              (FuncionCopiaGlist) copia_palabra_struct);
      tabla_revisadas =
          tablahash_inserta_encadenamiento(tabla_revisadas, newPalabra_struct);
    }
  }

  return estructura_validas;

}
