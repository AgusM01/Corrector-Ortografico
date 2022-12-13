El programa es un corrector de texto basado en los correctores ispell.
Utiliza 5 reglas de correccion, siendo estas:

*   Intercambiar cada par de caracteres adyacentes en la palabra.

*   Insertar cada letra de la ‘A’ a la ‘Z’ en cualquier posici ́on de la palabra (al principio, entre dos
caracteres, o al final).

*   Eliminar cada caracter de la palabra.

*   Reemplazar cada caracter de la palabra con cada letra de la ‘A’ a la ‘Z’.

*   Separar la palabra en un par de palabras agregando un espacio entre cada par de caracteres
adyacentes en la palabra. En este caso, se deber ́ıa sugerir si ambas palabras pertecen al diccionario.

El programa utiliza un diccionario el cual contendrá las palabras validas.
Se irá recorriendo un archivo de entrada palabra por palabra para ir checkeando que cada una de estas esté
en el diccionario. 
Si alguna de estas no está, se intenta corregir utilizando las reglas de correccion.
Se dejará de corregir una palabra cuando se encuentren 5 sugerencias validas de la misma o,
cuando se alcance el maximo de distancia a corregir de la palabra, en este caso 3.
Es decir, si se tiene por ejemplo la palabra kacza y se quiere obtener casa:
kacza -> cacza -> caza -> casa
En este caso, cazca estaria a distancia 1, caza a distancia 2 y casa a distancia 3.
Ya si a distancia 3 todavia no llegó a la palabra buscada, corta la búsqueda.

PARA COMPILAR: 
Simplemente para compilar el programa se debe escribir "make".

PARA EJECUTAR:
El programa se ejecuta escribiendo: ./corrector seguido de:
1er argumento: nombre del archivo de entrada (a corregir).
2do argumento: nombre del archivo de salida (donde se guardan las correcciones).
3er argumento: nombre del diccionario.

ARCHIVO CACHE:
El programa cuenta con un archivo cache (cache.txt). En este archivo se guardan las 
palabras corregidas, la cantidad de sugerencias encontradas y las sugerencias, de esta forma:

palabra, nro sugerencias, sugerencia1, ..., sugerenciaN

Este archivo se irá actualizando en cada ejecucion agregando todas las palabras que se 
hayan corregido.
De esta forma, si se quieren volver a corregir alguna de estas palabras, esta ya estará corregida 
en el cache y se extrae de ahi.
