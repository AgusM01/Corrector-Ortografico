#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hash.h"
#include "glist.h"
#include "PalabraStruct.h"
#include "Validas.h"

/*Regla la cual intercambia dos caracteres adyacentes de una palabra*/
Validas intercambiar(TablaHash tabla, Validas estructura_validas,
                     TablaHash tabla_revisadas, GPunteros lista_invalidas,
                     PalabraStruct pal_invalida, int store_words) {
  if (estructura_validas->cant == 5)
    return estructura_validas;

  char c;
  for (int i = 0; i < pal_invalida->length_palabra; i++) {
    if (pal_invalida->palabra[i + 1] != '\0') {
      c = pal_invalida->palabra[i];
      pal_invalida->palabra[i] = pal_invalida->palabra[i + 1];
      pal_invalida->palabra[i + 1] = c;
      estructura_validas =
          checkeo(estructura_validas, tabla, tabla_revisadas, lista_invalidas,
                  pal_invalida, store_words);
      pal_invalida->palabra[i + 1] = pal_invalida->palabra[i];
      pal_invalida->palabra[i] = c;

      if (estructura_validas->cant == 5)
        return estructura_validas;
    } else
      return estructura_validas;
  }
  return estructura_validas;
}

/*
 *Regla la cual inseta un carater de la a a la z entre cada par 
 *de caracteres adyacentes de una palabra.
*/
Validas insertar(TablaHash tabla, Validas estructura_validas,
                 TablaHash tabla_revisadas, GPunteros lista_invalidas,
                 PalabraStruct pal_invalida, int store_words) {
  if (estructura_validas->cant == 5)
    return estructura_validas;

  char c;
  int k = 1;
  char *newPalabra = malloc(sizeof(char) * (pal_invalida->length_palabra + 2));
  assert(newPalabra != NULL);
  newPalabra[0] = ' ';

  for (int i = 0; i <= pal_invalida->length_palabra; i++) {
    newPalabra[k] = pal_invalida->palabra[i];
    k++;
  }

  for (int i = 0; i <= pal_invalida->length_palabra; i++) {
    for (k = 'a'; k <= 'z'; k++) {
      newPalabra[i] = (char) k;
      PalabraStruct newPalabra_struct =
          crea_palabra_struct(newPalabra, pal_invalida->length_palabra + 1);
      estructura_validas =
          checkeo(estructura_validas, tabla, tabla_revisadas, lista_invalidas,
                  newPalabra_struct, store_words);
      destruye_palabra_struct_final(newPalabra_struct);

      if (estructura_validas->cant == 5) {
        free(newPalabra);
        return estructura_validas;
      }
    }
    c = newPalabra[i];
    newPalabra[i] = newPalabra[i + 1];
    newPalabra[i + 1] = c;
  }
  free(newPalabra);
  return estructura_validas;
}

/*Regla la cual elimina un caracter de una palabra*/
Validas eliminar(TablaHash tabla, Validas estructura_validas,
                 TablaHash tabla_revisadas, GPunteros lista_invalidas,
                 PalabraStruct pal_invalida, int store_words) {
  if (estructura_validas->cant == 5)
    return estructura_validas;

  char *newPalabra = malloc(sizeof(char) * (pal_invalida->length_palabra));
  assert(newPalabra != NULL);
  int n = 0;
  for (int i = 0; i < pal_invalida->length_palabra; i++) {
    for (int j = 0; j < pal_invalida->length_palabra; j++) {
      if (j == i) {
        newPalabra[j] = pal_invalida->palabra[n + 1];
        n++;
      } else
        newPalabra[j] = pal_invalida->palabra[n];
      n++;
    }
    n = 0;
    PalabraStruct newPalabra_struct =
        crea_palabra_struct(newPalabra, pal_invalida->length_palabra - 1);
    estructura_validas =
        checkeo(estructura_validas, tabla, tabla_revisadas, lista_invalidas,
                newPalabra_struct, store_words);
    if (estructura_validas->cant == 5) {
      destruye_palabra_struct_final(newPalabra_struct);
      free(newPalabra);
      return estructura_validas;
    }
    destruye_palabra_struct_final(newPalabra_struct);
  }
  free(newPalabra);
  return estructura_validas;
}

/*Regla la cual reemplaza cada caracter de la palabra por uno de la a la z*/
Validas reemplazar(TablaHash tabla, Validas estructura_validas,
                   TablaHash tabla_revisadas, GPunteros lista_invalidas,
                   PalabraStruct pal_invalida, int store_words) {
  if (estructura_validas->cant == 5)
    return estructura_validas;

  char reserva;
  for (int i = 0; i < pal_invalida->length_palabra; i++) {
    reserva = pal_invalida->palabra[i];
    for (int k = 'a'; k <= 'z'; k++) {
      pal_invalida->palabra[i] = (char) k;
      estructura_validas =
          checkeo(estructura_validas, tabla, tabla_revisadas, lista_invalidas,
                  pal_invalida, store_words);

      if (estructura_validas->cant == 5) {
        pal_invalida->palabra[i] = reserva;
        return estructura_validas;
      }

    }
    pal_invalida->palabra[i] = reserva;
  }
  return estructura_validas;

}

/*
 *Regla la cual agrega un espacio entre cada par adyacentes de una palabra.
 *Esta regla debe checkear que ambas mitades de la palabra separada
 *esten en el diccionario. De ser así, agrega la palabra obtenida
 *luego de agregar el espacio entre ambas mitades.
*/
Validas separar(TablaHash tabla, Validas estructura_validas,
                PalabraStruct pal_invalida) {
  if (estructura_validas->cant == 5)
    return estructura_validas;

  char *palabra_separada =
      malloc(sizeof(char) * (pal_invalida->length_palabra + 2));
  assert(palabra_separada != NULL);
  palabra_separada[1] = '\0';
  palabra_separada[pal_invalida->length_palabra + 1] = '\0';
  int flag = 1, k = 0;

  for (int i = 0; i < pal_invalida->length_palabra; i++) {
    if (i == 1)
      k++;
    palabra_separada[k] = pal_invalida->palabra[i];
    k++;
  }

  for (int i = 0; i < pal_invalida->length_palabra - 1; i++) {
    PalabraStruct mitad1 = crea_palabra_struct(palabra_separada, i + 1);
    PalabraStruct mitad2 =
        crea_palabra_struct(&palabra_separada[i + 2],
                            pal_invalida->length_palabra - (i + 1));

    if (estructura_validas->cant == 5) {
      free(palabra_separada);
      destruye_palabra_struct_final(mitad1);
      destruye_palabra_struct_final(mitad2);
      return estructura_validas;
    }
    if (tablahash_buscar(tabla, mitad1) != NULL &&
        tablahash_buscar(tabla, mitad2) != NULL) {
      palabra_separada[i + 1] = ' ';
      for (int j = 0; j < estructura_validas->cant; j++) {
        flag =
            strcmp(palabra_separada, estructura_validas->lista_sugerencias[j]);
      }
      if (flag != 0)
        estructura_validas =
            agrega_sugerencia_validas(estructura_validas, palabra_separada);
      palabra_separada[i + 1] = '\0';
    }
    palabra_separada[i + 1] = palabra_separada[i + 2];
    palabra_separada[i + 2] = '\0';
    destruye_palabra_struct_final(mitad1);
    destruye_palabra_struct_final(mitad2);
  }
  free(palabra_separada);
  return estructura_validas;
}
