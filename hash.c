#include <stdio.h>
#include "hash.h"
#include <assert.h>
#include <stdlib.h>
#include "glist.h"

/**
 * Casillas que contendran listas enlazadas para resolver encadenamiento.
 */
typedef struct {
  GPunteros dato;
} CasillaHash;

/**
 * Estructura secundaria que representa la tabla hash
 * usando CasillaHash.
 */
struct _TablaHash {
  CasillaHash *elems;
  unsigned numElems;
  unsigned capacidad;
  FuncionCopiadora copia;
  FuncionComparadora comp;
  FuncionHash hash;
  FuncionVisitante visit;
  FuncionDatoDestructora destrDato;
};


/**
 * Crea una nueva tabla hash vacia, con la capacidad dada.
 */
TablaHash tablahash_crear(unsigned capacidad, FuncionCopiadora copia,
                          FuncionComparadora comp,
                          FuncionHash hash, FuncionVisitante visit,
                          FuncionDatoDestructora destrDato) {

  // Pedimos memoria para la estructura principal y las casillas.
  TablaHash tabla = malloc(sizeof(struct _TablaHash));
  assert(tabla != NULL);
  tabla->elems = malloc(sizeof(CasillaHash) * capacidad);
  assert(tabla->elems != NULL);
  tabla->numElems = 0;
  tabla->capacidad = capacidad;
  tabla->copia = copia;
  tabla->comp = comp;
  tabla->hash = hash;
  tabla->visit = visit;
  tabla->destrDato = destrDato;
  // Inicializamos las casillas con datos nulos.
  for (int idx = 0; idx < (int) capacidad; ++idx)
    tabla->elems[idx].dato = glist_crear_punteros();
  return tabla;
}

/**
 * Retorna el numero de elementos de la tabla.
 */
int tablahash_nelems(TablaHash tabla) {
  return tabla->numElems;
}

/**
 * Retorna la capacidad de la tabla.
 */
int tablahash_capacidad(TablaHash tabla) {
  return tabla->capacidad;
}

/**
 * Destruye la tabla.
 */
void tablahash_destruir(TablaHash tabla) {

  // Destruir cada uno de los datos.
  for (int idx = 0; idx < (int) tabla->capacidad; ++idx) {
    if (tabla->elems[idx].dato != NULL)
      glist_destruir(tabla->elems[idx].dato, tabla->destrDato);
  }
  // Liberar el arreglo de casillas y la tabla.
  free(tabla->elems);
  free(tabla);
  return;
}

/**
 * Retorna el dato de la tabla que coincida con el dato dado, o NULL si el dato
 * buscado no se encuentra en la tabla.
 */
void *tablahash_buscar(TablaHash tabla, void *dato) {
  unsigned idx = tabla->hash(dato) % tabla->capacidad;
  return glist_buscar(tabla->elems[idx].dato->primero, dato, tabla->comp);
}

/*
 * Inserta el dato dado en la casilla hash, agregando este dato a la 
 * lista enlazada presente en dicho casillero.
*/
TablaHash tablahash_inserta_encadenamiento(TablaHash tabla, void *dato) {

  //Se calcula la posicion del dato dado, de acuerdo a la funcion hash.
  unsigned idx = tabla->hash(dato) % tabla->capacidad;

  //Calculamos si es necesario realizar un rehash
  if (10 * tablahash_nelems(tabla) > 7 * tablahash_capacidad(tabla)) {
    tabla = tablahash_rehash(tabla);
  }
  //Busca el dato en la tabla, si está, no hace nada, si no está, lo agrega.
  if (glist_buscar(tabla->elems[idx].dato->primero, dato, tabla->comp) == NULL) {
    tabla->numElems++;
    tabla->elems[idx].dato =
        glist_agregar_final(tabla->elems[idx].dato, dato, tabla->copia);
  }
  return tabla;
}

/*
 *Imprime la tabla hash
*/
void imprimir_tablahash(TablaHash tabla) {
  for (int i = 0; i < (int) tabla->capacidad; i++) {
    printf("Elemento: %d, dato:", i);
    printf("\t");
    glist_recorrer(tabla->elems[i].dato, (FuncionVisitanteGlist) tabla->visit);
    printf("\n");

  }
}

/*
 *Realiza el rehash de la tabla hash.
*/
TablaHash tablahash_rehash(TablaHash tabla) {

  /* La idea es intercambiar solamente el arreglo de elementos por uno nuevo 
     con mayor capadidad utilizando la misma tabla.
   */
  int capacidad_anterior = tabla->capacidad;    // Guardamos la capacidad anterior.
  tabla->capacidad *= 2;        //Duplicamos la capacidad de la tabla-
  CasillaHash *temp = tabla->elems;     //Creamos un arreglo temporal el cual le damos el valor del arreglo actual de la tabla.
  CasillaHash *newArray = malloc(sizeof(CasillaHash) * tabla->capacidad);       //Creamos un nuevo arreglo con la nueva capacidad.
  assert(newArray != NULL);
  tabla->elems = newArray;      //Los elementos de la tabla estarán en este nuevo arreglo.
  tabla->numElems = 0;          //Por ahora hay cero elementos

  // Inicializamos las casillas con datos nulos.
  for (int idx = 0; idx < (int) tabla->capacidad; ++idx)
    tabla->elems[idx].dato = glist_crear_punteros();

  //Recorremos el arreglo anterior, guardando todos los elementos en el nuevo arreglo.
  //Como la capacidad cambió, el indice de hash de cada elemento tambien puede cambiar.
  for (int i = 0; i < capacidad_anterior; i++) {
    for (GList node = temp[i].dato->primero; node != NULL; node = node->next) {
      tabla = tablahash_inserta_encadenamiento(tabla, node->data);
    }
    glist_destruir(temp[i].dato, tabla->destrDato);
  }
  free(temp);
  return tabla;
}
