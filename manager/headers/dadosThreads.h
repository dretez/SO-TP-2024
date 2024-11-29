#ifndef INCLUDE_HEADERS_DADOSTHREADS_H_
#define INCLUDE_HEADERS_DADOSTHREADS_H_

#include <pthread.h>

#include "../../include/communication.h"
#include "dados.h"

typedef struct {
  packet *p;
  int *cont;
} TDATA;

// Routinas de threads
void *admin_thread(void *args);
void *counter_thread(void *args);

#endif // INCLUDE_HEADERS_DADOSTHREADS_H_
