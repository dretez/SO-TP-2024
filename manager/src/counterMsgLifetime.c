#include "../headers/dadosThreads.h"

void *counter_thread(void *args) {
  int *cont = (int *)args;
  while (*cont) {
  }

  pthread_exit(NULL);
}