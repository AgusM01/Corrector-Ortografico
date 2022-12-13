#ifndef __PALABRASTRUCT_H__
#define __PALABRASTRUCT_H__

/*Estructura tipo Palabra la cual tiene una palabra en cuestión y su longitud*/
struct _Palabra {
    char* palabra;
    int length_palabra;

};

typedef struct _Palabra *PalabraStruct;

/*
 *Crea una estructura palabra
*/
PalabraStruct crea_palabra_struct(char* palabra, int length_palabra);

/*
 *Compara si dos estructuras palabras son iguales en base a la palabra
    que contienen.
 *Si son iguales devuelve 0.
*/
int compara_palabra_struct(PalabraStruct str1, PalabraStruct str2);

/*
 * Copia una  estructra palabra.
*/
PalabraStruct copia_palabra_struct(PalabraStruct str);

/*
 *Calcula el indice de hash de una estructra palabra
*/
unsigned hash_palabra_struct(PalabraStruct str);

/*
 * Imprime una estructura palabra.
*/
void imprime_palabra_struct(PalabraStruct dato);

/*
 * Destruye una estructura palabra.
*/
void destruye_palabra_struct_final(PalabraStruct str);

#endif