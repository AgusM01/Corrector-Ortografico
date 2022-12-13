#ifndef __REGLAS_H__
#define __REGLAS_H__

typedef struct _TablaHash *TablaHash;
typedef struct _Palabra *PalabraStruct;
typedef struct _Validas *Validas;
typedef struct _GPunteros *GPunteros;

/*Regla la cual intercambia dos caracteres adyacentes de una palabra*/
Validas intercambiar(TablaHash tabla, Validas estructura_validas, TablaHash tabla_revisadas, GPunteros lista_invalidas, PalabraStruct pal_invalida, int store_words);
/*
 *Regla la cual inseta un carater de la a a la z entre cada par 
 *de caracteres adyacentes de una palabra.
*/
Validas insertar(TablaHash tabla, Validas estructura_validas, TablaHash tabla_revisadas, GPunteros lista_invalidas, PalabraStruct pal_invalida, int store_words);
/*Regla la cual elimina un caracter de una palabra*/
Validas eliminar(TablaHash tabla, Validas estructura_validas, TablaHash tabla_revisadas, GPunteros lista_invalidas, PalabraStruct pal_invalida, int store_words);
/*Regla la cual reemplaza cada caracter de la palabra por uno de la a la z*/
Validas reemplazar(TablaHash tabla, Validas estructura_validas, TablaHash tabla_revisadas, GPunteros lista_invalidas, PalabraStruct pal_invalida, int store_words);
/*
 *Regla la cual agrega un espacio entre cada par adyacentes de una palabra.
 *Esta regla debe checkear que ambas mitades de la palabra separada
 *esten en el diccionario. De ser así, agrega la palabra obtenida
 *luego de agregar el espacio entre ambas mitades.
*/
Validas separar(TablaHash tabla, Validas estructura_validas, PalabraStruct pal_invalida);

#endif /* __TABLAHASH_H__ */