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

#endif // !SHARED_GLOBALS_H
