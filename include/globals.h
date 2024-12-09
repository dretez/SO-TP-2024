#ifndef SHARED_GLOBALS_H
#define SHARED_GLOBALS_H

// Includes
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// pipes
#define FIFO_SRV "srv_pipe"
#define FIFO_CLI "cli_%d"

/* Devolve o tamanho de uma palavra, ou seja, o # de carateres até um espaço em
 * branco */
int wordlen(char *wrd);
/* Dado um ponteiro para uma string e um offset desse pointeiro, devolve o
 * offset/index de início da próxima palavra na string.
 * Se nenhuma palavra for encontrada, devolve -1. */
int nextword(char *str, int offset, int size);
/* Copia uma palavra, ou seja, copia carateres até encontrar um espaço em
 * branco, ou copia até o limite indicado por size. */
int wordncpy(char *dest, char *orig, int size);

#endif // !SHARED_GLOBALS_H
