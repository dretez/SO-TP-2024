#include <stdio.h>
#include <stdlib.h>

#include "../../include/globals.h"

// Limites predefinidos
#define MAX_USERS 10
#define MAX_TOPICS 20
#define MAX_PERSISTENT_MSGS 5

int main(int argc, char *argv[]) {
  int fd, res;
  char str[30];
  // PEDIDO p;
  char fifo_cli[30];
  int fd_cli;
  // RESPOSTA r;

  printf("INICIO!!! \n");

  if (access(FIFO_SRV, F_OK) == 0) {
    printf("[ERRO] Ja existe um servidor!\n");
    exit(3);
  }
  mkfifo(FIFO_SRV, 0600);
  fd = open(FIFO_SRV, O_RDWR);

  /*
   do {
   // res = read(fd, &p, sizeof(PEDIDO));

   // sprintf(fifo_cli, FIFO_CLI, p.pid);
   fd_cli = open(fifo_cli, O_WRONLY);
   // strcpy(r.str, "OK");
   // res = write(fd_cli, &r, sizeof(RESPOSTA));
   // printf(" ENVIEI ... '%s'(%d)\n", r.str, res);
   close(fd_cli);
} while (strcmp(p.str, "quit") != 0);
  */
  close(fd);
  unlink(FIFO_SRV);
  printf("FIM \n");
  exit(0);
}
