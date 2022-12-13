#ifndef __TABLAHASH_H__
#define __TABLAHASH_H__

typedef void *(*FuncionCopiadora)(void *dato);
/** Retorna una copia fisica del dato */
typedef int (*FuncionComparadora)(void *dato1, void *dato2);
/** Retorna un entero negativo si dato1 < dato2, 0 si son iguales y un entero
 * positivo si dato1 > dato2  */
typedef unsigned (*FuncionHash)(void *dato);
/*Funcion hash encargada de asignarle un indice a cada elemento.*/
typedef void (*FuncionVisitante)(void* dato);
/*Funcion visitante especifica del tipo de dato a guardar*/
typedef void (*FuncionDatoDestructora)(void* dato);
/*Funcion destructora especifica del tipo de dato a guardar*/


typedef struct _TablaHash *TablaHash; /*Puntero a estructura _TablaHash*/



/**
 * Crea una nueva tabla hash vacia, con la capacidad dada.
 */
TablaHash tablahash_crear(unsigned capacidad, FuncionCopiadora copia,
                          FuncionComparadora comp, FuncionHash hash, 
                          FuncionVisitante visit,
                          FuncionDatoDestructora destrDato);

/**
 * Retorna el numero de elementos de la tabla.
 */ 
int tablahash_nelems(TablaHash tabla);

/**
 * Retorna la capacidad de la tabla.
 */
int tablahash_capacidad(TablaHash tabla);

/**
 * Destruye la tabla.
 */
void tablahash_destruir(TablaHash tabla);

/**
 * Inserta un dato en la tabla, o lo reemplaza si ya se encontraba.
 */
TablaHash tablahash_inserta_encadenamiento(TablaHash tabla, void *dato);

/**
 * Retorna el dato de la tabla que coincida con el dato dado, o NULL si el dato
 * buscado no se encuentra en la tabla.
 */
void *tablahash_buscar(TablaHash tabla, void *dato);

/*
 * Imprime la tabla hash.
*/
void imprimir_tablahash(TablaHash tabla);

/*
 * Realiza el rehash de todos los elementos.
*/
TablaHash tablahash_rehash(TablaHash tabla);

#endif /* __TABLAHASH_H__ */