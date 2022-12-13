#include "utils.h"
/*Funcion hash djb2 encontrada en internet*/
unsigned djb2(char *str) {
  unsigned long hash = 5381;
  int c;

  while ((c = *str++))
    hash = ((hash << 5) + hash) + c;    /* hash * 33 + c */

  return hash;
}
