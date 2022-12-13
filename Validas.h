#ifndef __VALIDAS_H__
#define __VALIDAS_H__
#define MAX_SUG 5


typedef struct _GNode *GList;
typedef struct _GPunteros *GPunteros;
typedef struct _TablaHash *TablaHash;
typedef struct _Palabra *PalabraStruct;

/*
 *Estructura validas la cual contiene:
    *La palabra invalida a corregir,
    *El eamaño de esta palabra,
    *La cantidad de sugerencias,
    *Las sugerencias encontradas.
*/
struct _Validas{
    char* pal_invalida;
    int pal_invalida_length;
    int cant;
    char* lista_sugerencias[MAX_SUG];
};

typedef struct _Validas *Validas;

/*
 *Crea una estructura validas.
*/
Validas crea_validas(char* pal_invalida, int pal_invalida_length);
/*
 *Destruye una estructura validas.
*/
void destruye_validas(Validas estructura_validas);
/*
 *Agrega una sugerencia de la palabra a su estructura.
*/
Validas agrega_sugerencia_validas(Validas estructura_validas, char* sugerencia);
/*
 *Compara si dos estructuras validas son iguales en base a su palabra invalida.
 *Devuelve 0 si son iguales, otro entero caso contrario (implementacion de strcmp).
*/
int compara_validas(Validas estructura_validas1, Validas estructura_validas2);
/*
 *Copia una estructura validas.
*/
Validas copia_validas(Validas estructura_validas);
/*
 *Calcula el indice de hash de una estructura validas.
*/
unsigned hash_validas(Validas estructura_validas);
/*
 *Imprime una estrucura validas.
*/
void imprime_validas(Validas estructura_validas);
/*
 *Funcion la cual se encargara de tomar una palabra y corregirla,
 *aplicando las reglas de correccion sobre la misma y sus generaciones en caso de no 
 *encontrar 5 sugerencias.
 *Esto se hace hasta una distancia de "generacion" de 3.
 *Si luego de analizar la 3ra generacion no se encuentran 5 sugerencias,
 *se muestran las que se encontraron.
*/
Validas corregir(Validas palabra_valida,PalabraStruct palabra, TablaHash diccionario);
/*
 *Checkea si una palaba generada mediante una de las reglas es valida para que 
 *sea una sugerencia (está en el diccionario).
 *Además, en el caso de no estar trabajando en la 2da generacion de sucesores,
 *agrega las palabras generadas a la lista de los proximos sucesores a analizar.
*/
Validas checkeo (Validas estructura_validas, TablaHash tabla, 
                 TablaHash tabla_revisadas, GPunteros lista_invalidas,
                 PalabraStruct newPalabra_struct, int store_words);
                 
#endif //__VALIDAS_H__