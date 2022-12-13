#include "glist.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * Devuelve una lista vacía.
 */
GList glist_crear() {
  return NULL;
}

/**
 * Crea una estructura GPunteros.
 */
GPunteros glist_crear_punteros() {
  GPunteros list = malloc(sizeof(struct _GPunteros));
  assert(list != NULL);
  list->primero = NULL;
  list->ultimo = NULL;
  return list;
}

/**
 * Destruccion de la lista.
 * destroy es una función que libera el dato almacenado.
 */
void glist_destruir(GPunteros list, FuncionDestructoraGlist destroy) {
  GNode *nodeToDelete;
  while (list->primero != NULL) {
    nodeToDelete = list->primero;
    list->primero = nodeToDelete->next;
    destroy(nodeToDelete->data);
    free(nodeToDelete);
  }
  free(list);
}

/*
 *Agrega una copia del dato dado al final de la lista.
*/
GPunteros glist_agregar_final(GPunteros list, void *data,
                              FuncionCopiaGlist copy) {
  GNode *newNode = malloc(sizeof(GNode));
  assert(newNode != NULL);
  newNode->data = copy(data);

  if (list->primero == NULL) {
    list->primero = newNode;
    list->ultimo = list->primero;
    newNode->next = NULL;
    return list;
  }

  newNode->next = NULL;
  list->ultimo->next = newNode;
  list->ultimo = newNode;
  return list;
}

/**
 * Recorrido de la lista, utilizando la funcion pasada.
 */
void glist_recorrer(GPunteros list, FuncionVisitanteGlist visit) {
  GList temp = list->primero;
  for (; temp != NULL; temp = temp->next) {
    visit(temp->data);
  }
}

/**
 * Busca un elemento en la lista, si está devuelve ese elemento,
 * si no, devuelve NULL.
 */
void *glist_buscar(GList list, void *dato, FuncionComparadoraGList compara) {

  for (GList nodo = list; nodo != NULL; nodo = nodo->next) {
    if (compara(dato, nodo->data) == 0)
      return nodo->data;
  }
  return NULL;
}
