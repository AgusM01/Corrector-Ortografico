#ifndef __GLIST_H__
#define __GLIST_H__

typedef int (*FuncionComparadoraGList) (void* dato1, void* dato2);
typedef void (*FuncionDestructoraGlist)(void *dato);
typedef void *(*FuncionCopiaGlist)(void *dato);
typedef void (*FuncionVisitanteGlist)(void *dato);
typedef int (*PredicadoGlist) (void *dato); //Se declara puntero a void y luego se le puede pasar cualquier
                                       //puntero para poder usarla para cualquier tipo de dato.

/*
  Estructura utilizada para trabajar sobre listas enlazadas.
  Se guarda un puntero al primer elemento de la lista y uno al ultimo
  los cuales nos servirán para el caso de querer agregar al final.
*/
typedef struct _GPunteros{
  struct _GNode *primero;
  struct _GNode *ultimo;
} *GPunteros;

/*
  Estructura utilizada para listas enlazadas generales.
*/
typedef struct _GNode {
  void *data;
  struct _GNode *next;
} GNode;

typedef GNode *GList;


/**
 * Devuelve una estructura GPunteros con sus datos inicializados en NULL.
 */
GPunteros glist_crear_punteros();
/**
 * Devuelve una lista vacía.
 */
GList glist_crear();

/**
 * Destruccion de la lista.
 */
void glist_destruir(GPunteros list, FuncionDestructoraGlist destroy);

/*
 * Agrega un elemento al final de la lista,
*/
GPunteros glist_agregar_final(GPunteros list, void *data, FuncionCopiaGlist copy);
/**
 * Recorrido de la lista, utilizando la funcion pasada.
 */
void glist_recorrer(GPunteros lista, FuncionVisitanteGlist visitar);

/**
 * Busca un elemento en la lista, si está devuelve ese elemento,
 * si no, devuelve NULL.
 */
void* glist_buscar(GList lista, void *dato, FuncionComparadoraGList compara);
#endif /* __GLIST_H__ */