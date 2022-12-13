#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "utils.h"
#include "Validas.h"
#include "glist.h"
#include "PalabraStruct.h"
#include "hash.h"


#define CANTIDAD_CHAR_INICIAL 20

/*Funcion que lee una palabra de un archivo*/
char *toma_palabra(FILE * fp, int *c, int *length_palabra, int *cant_barra_n) {
  int cant_char_variable = CANTIDAD_CHAR_INICIAL, i = 0;
  char *palabra = malloc(sizeof(char) * CANTIDAD_CHAR_INICIAL);
  assert(palabra != NULL);
  *c = fgetc(fp);
  while (isalpha(*c)) {         //Checkea que sea una palabra.
    if (i == cant_char_variable - 1) {
      cant_char_variable *= 2;
      palabra = realloc(palabra, cant_char_variable);
    }
    if (isupper(*c))            //Si es mayúscula, la pasa a minúscula.
      *c = tolower(*c);
    palabra[i] = *c;
    i++;
    *c = fgetc(fp);
    if ((*c == ':') || (*c == ';') || (*c == ',')
        || (*c == '.') || (*c == '?') || (*c == '!')) {
      *c = fgetc(fp);
    }
  }
  if (*c == 10)                 //Si hay un \n, lo suma a la cantidad. Sirve para el archivo de salida.
    *cant_barra_n += 1;
  palabra[i] = '\0';
  *length_palabra = i;
  return palabra;
}

/*Funcion que lee una linea del archivo cache (la palabra, cantidad de sugerencias y sugerencias)*/
Validas toma_palabra_cache(FILE * fp, int *c) {
  int length_palabra = 0, k = 0;
  char *palabra_invalida = toma_palabra(fp, c, &length_palabra, &k);    //Toma la palabra que se corrigió previamente.
  Validas palabra_cache = crea_validas(palabra_invalida, length_palabra);       //Crea un tipo de dato válida para asi poder tener la palabra y sus sugerencias.
  free(palabra_invalida);
  while (!isdigit(*c))          //Lee hasta que no sea un dígito.
    *c = fgetc(fp);

  int cant = *c - '0';          //c entonces ahora es un dígito pero en char, por lo que lo transforma a int y lo guarda en cant.
  assert(cant >= 0);            //Se asegura que la cantidad de sugerencias no sea negativo.
  palabra_cache->cant = cant;   //Agrego a la estructura valida creada la cantidad de sugerencias.

  if (cant == 0) {              //Si no tiene sugerencias, simplemente leo hasta que sea \n o EOF y retorno.S
    while (*c != '\n' && *c != EOF)
      *c = fgetc(fp);
    return palabra_cache;
  }
  while (!isalpha(*c))          //Si tiene sugerencias, adelanto hasta que encuentre una (el comienzo de una).S
    *c = fgetc(fp);

  ungetc(*c, fp);               //Devuelvo el caracter ya que toma_palabra al principio lo debe consumir.
  for (int i = 0; i < cant; i++) {      //Voy leyendo cada sugerencia.
    palabra_cache->lista_sugerencias[i] =
        toma_palabra(fp, c, &length_palabra, &k);
    if (i != cant - 1) {        //Si llego a la ultima debo parar de leer.
      while (!isalpha(*c))
        *c = fgetc(fp);
      ungetc(*c, fp);
    } else
      i = cant;
  }
  //Consumo caracteres hasta que sea \n o EOF
  while (*c != '\n' && *c != EOF)
    *c = fgetc(fp);
  return palabra_cache;
}

/*Funcion que lee el diccionario y guarda sus palabras en una tabla hash*/
void lee_diccionario(TablaHash diccionario, char *fdiccionario) {
  FILE *file_dict;
  file_dict = fopen(fdiccionario, "r");
  assert(file_dict != NULL);
  int c = 0, length_palabra = 0;
  char *palabra_archivo;
  int n = 0, cant_barra_n = 0;
  while (c != EOF) {
    palabra_archivo = toma_palabra(file_dict, &c, &length_palabra, &cant_barra_n);      //Tomo una palabra.
    PalabraStruct palabra_estructura = crea_palabra_struct(palabra_archivo, length_palabra);    //Creo una estructura palabra de la misma (palabra y su tamaño).
    if (length_palabra != 0) {  //Si la longitud es distinta de cero, la agrega al diccionario.
      diccionario =
          tablahash_inserta_encadenamiento(diccionario, palabra_estructura);
      n++;
    }
    free(palabra_archivo);
    destruye_palabra_struct_final(palabra_estructura);
    length_palabra = 0;
  }
  fclose(file_dict);            //Cierro el archivo.
  return;
}

/*Funcion que lee el archivo cache y guarda sus palabras y las sugerencias de cada una en la tabla de palabras validas*/
void lee_cache(FILE * f_cache, TablaHash tabla_validas) {
  int c = 0;
  while (c != EOF) {
    c = fgetc(f_cache);         //Consume el primer caracter de la palabra.
    if (c != EOF) {             //Checkea si no es EOF.
      ungetc(c, f_cache);       //Lo devuelve al flujo de lectura.
      Validas palabra_cache = toma_palabra_cache(f_cache, &c);  //Crea la estructura la cual guardará la palabra y sus sugerencias.
      if (palabra_cache->pal_invalida_length != 0) {    //Checkea que la palabra no tenga longitud cero.
        tabla_validas = tablahash_inserta_encadenamiento(tabla_validas, palabra_cache); //Agrega la palabra a la tabla de palabras validas.
      }
      destruye_validas(palabra_cache);
    }
  }

  return;
}

/*Funcion que toma las palabras del archivo de entrada, las corrige y las escribe en el archivo de salida*/
void escribe_salida(FILE * f_entrada, FILE * f_cache, FILE * f_salida,
                    TablaHash diccionario, TablaHash tabla_validas) {
  int length_palabra = 0, c = 0, barras_salida = 1;
  char *palabra_archivo;
  int cant_barra_n = 1;

  while (c != EOF) {
    palabra_archivo = toma_palabra(f_entrada, &c, &length_palabra, &cant_barra_n);      //Tomo una palabra del archivo
    if (length_palabra != 0) {  //Checkeo que tenga longitud distinta de cero (no puede ser negativa).
      PalabraStruct palabra_struct = crea_palabra_struct(palabra_archivo, length_palabra);      //Crea una estructura palabra con la palabra
      if (tablahash_buscar(diccionario, palabra_struct) == NULL) {      //Si la palabra no está en el diccionario, se debe corregir.
        Validas palabra_validas = crea_validas(palabra_archivo, length_palabra);        //Crea una estructura valida con la palabra
        Validas palabra_busqueda = tablahash_buscar(tabla_validas, palabra_validas);    //Si la palabra está, la guarda en palabra_busqueda
        fprintf(f_salida, "Linea %d, '%s' no está en el diccionario.\n",
                barras_salida, palabra_archivo);
        fprintf(f_salida, "Quizas quiso decir: ");
        if (palabra_busqueda == NULL) { //si la busqueda dió NULL, la palabra no la corregí previamente, por lo tanto la debo corregir.
          palabra_validas = corregir(palabra_validas, palabra_struct, diccionario);     //Llama a funcion corregir la cual corrige la palabra.
          tabla_validas =
              tablahash_inserta_encadenamiento(tabla_validas, palabra_validas);
          fprintf(f_cache, "%s, ", palabra_validas->pal_invalida);
          fprintf(f_cache, "%d, ", palabra_validas->cant);
          for (int i = 0; i < palabra_validas->cant; i++)
            fprintf(f_cache, "%s, ", palabra_validas->lista_sugerencias[i]);
          fprintf(f_cache, "\n");
        } else {
          destruye_validas(palabra_validas);    //Si la corregí previamente, destruyo la estructura. 
          palabra_validas = copia_validas(palabra_busqueda);    //Guardo una copia de la encontrada.
        }
        for (int i = 0; i < palabra_validas->cant; i++) {       //Imprimo sus sugerencias en el archivo de salida.
          if (i == palabra_validas->cant - 1)
            fprintf(f_salida, "%s. ", palabra_validas->lista_sugerencias[i]);
          else
            fprintf(f_salida, "%s, ", palabra_validas->lista_sugerencias[i]);
        }
        fprintf(f_salida, "\n");
        fprintf(f_salida, "\n");
        destruye_validas(palabra_validas);
      }
      destruye_palabra_struct_final(palabra_struct);

    }
    free(palabra_archivo);
    barras_salida = cant_barra_n;       //Llevo una cuenta de los \n para saber en que linea está la palabra a corregir.
    length_palabra = 0;
  }
  return;
}
