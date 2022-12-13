#ifndef __ARCHIVO_H__
#define __ARCHIVO_H__

typedef struct _Validas *Validas;
typedef struct _TablaHash *TablaHash;

/*Funcion que lee una palabra de un archivo*/
char* toma_palabra(FILE* fp, int* c, int* length_palabra, int* cant_barra_n);
/*Funcion que lee el diccionario y guarda sus palabras en una tabla hash*/
void lee_diccionario(TablaHash tabla, char* fdiccionario);
/*Funcion que lee el archivo cache y guarda sus palabras y las sugerencias de cada una en la tabla de palabras validas*/
void lee_cache(FILE* f_cache, TablaHash tabla_validas);
/*Funcion que lee una linea del archivo cache (la palabra, cantidad de sugerencias y sugerencias)*/
Validas toma_palabra_cache(FILE* fc, int* c);
/*Funcion que toma las palabras del archivo de entrada, las corrige y las escribe en el archivo de salida*/
void escribe_salida(FILE* f_entrada, FILE* f_cache, FILE* f_salida, TablaHash diccionario, TablaHash tabla_validas);
#endif