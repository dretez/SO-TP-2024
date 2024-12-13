#ifndef INCLUDE_HEADERS_DADOSTHREADS_H_
#define INCLUDE_HEADERS_DADOSTHREADS_H_

#include <pthread.h>

#include "../../include/communication.h"
#include "dados.h"

#define TCOUNT 2

typedef struct {
  packet *p;
  int *cont;
  int *fifo_srv;
} TDATA;

void stopManager();

// Routinas de threads
void *admin_thread(void *args);
void *counter_thread(void *args);

#endif // INCLUDE_HEADERS_DADOSTHREADS_H_
