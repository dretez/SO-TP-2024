#include "../headers/dadosThreads.h"

void *counter_thread(void *args) {
  int *cont = (int *)args;
  while (*cont) {
    sleep(1);
  }

  pthread_exit(NULL);
}
