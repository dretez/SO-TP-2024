#include "../headers/dadosThreads.h"

void *counter_thread(void *args) {
  TDATA *td = (TDATA *)args;
  while (*td->cont) {
    sleep(1);
    writeEmptyPacket(td->p, P_TYPE_MNGR_DECREMENT);
    write(*td->fifo_srv, td->p, packetSize(*td->p));
  }

  pthread_exit(NULL);
}
