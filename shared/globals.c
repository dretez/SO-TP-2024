#include "../include/globals.h"

int wordlen(char *wrd) {
  int i;
  for (i = 0; wrd[i] != ' ' && wrd[i] != '\0' && wrd[i] != '\n'; i++) {
  }
  return i;
}

int nextword(char *str, int offset, int size) {
  int i = offset;
  for (; str[i] != ' ' && i < size; i++)
    ;
  if (i == size)
    return -1;

  for (; (str[i] == ' ' || str[i] == '\n') && i < size; i++)
    ;
  if (i == size || str[i] == '\0')
    return -1;

  return i;
}

int wordncpy(char *dest, char *orig, int size) {
  int i = 0;
  for (; orig[i] != ' ' && orig[i] != '\0' && orig[i] != '\n' && i < size;
       i++) {
    dest[i] = orig[i];
  }
  dest[i] = '\0';
  return i;
}
