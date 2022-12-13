#include <stdio.h>
#include <stdlib.h>
#include "PalabraStruct.h"
#include <string.h>
#include "utils.h"
#include <assert.h>

/*
 *Crea una estructura palabra
*/
PalabraStruct crea_palabra_struct(char *palabra, int length_palabra) {
  PalabraStruct estructura_palabra = malloc(sizeof(struct _Palabra));
  assert(estructura_palabra != NULL);
  estructura_palabra->palabra = strdup(palabra);
  estructura_palabra->length_palabra = length_palabra;
  return estructura_palabra;
}

/*
 *Compara si dos estructuras palabras son iguales en base a la palabra
    que contienen.
 *Si son iguales devuelve 0.
*/
int compara_palabra_struct(PalabraStruct str1, PalabraStruct str2) {
  return strcmp(str1->palabra, str2->palabra);
}

/*
 * Copia una  estructra palabra.
*/
PalabraStruct copia_palabra_struct(PalabraStruct str) {
  PalabraStruct Palabra_copia =
      crea_palabra_struct(str->palabra, str->length_palabra);
  return Palabra_copia;
}

/*
 *Calcula el indice de hash de una estructra palabra
*/
unsigned hash_palabra_struct(PalabraStruct str) {
  return djb2(str->palabra);
}

/*
 * Imprime una estructura palabra.
*/
void imprime_palabra_struct(PalabraStruct dato) {
  printf("\t%s", dato->palabra);
}

/*
 * Destruye una estructura palabra.
*/
void destruye_palabra_struct_final(PalabraStruct str) {
  free(str->palabra);
  free(str);
  return;
}
