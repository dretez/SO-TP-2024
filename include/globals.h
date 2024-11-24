// Includes
#include <fcntl.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef SHARED_GLOBALS_H
#define SHARED_GLOBALS_H

// pipes
#define FIFO_SRV "srv_pipe"
#define FIFO_CLI "cli_%d"

// Macros
#define maxval(type)                                                           \
  (1 << sizeof(type) * CHAR_BIT) - 1 // obtém o valor maximo permitido por type
                                     // ou o equivalente a colocar todos os
                                     // bits de type a 1

#endif // !SHARED_GLOBALS_H
