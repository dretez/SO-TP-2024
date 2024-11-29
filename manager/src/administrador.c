#include "../headers/dadosThreads.h"

void *admin_thread(void *args) {
  TDATA *td = (TDATA *)args;
  packet *p = td->p;

  while (td->cont) {
    char fifo_cli[30];
    sprintf(fifo_cli, FIFO_CLI, p->head.pid);
    int fd_cli = open(fifo_cli, O_WRONLY);

    // prepara pacote a ser enviado

    int res = write(fd_cli, p, packetSize(*p));
    close(fd_cli);
  }

  pthread_exit(NULL);
}

void processCmd(packet *p, char *input, char *username) {
  char cmd[12];
  sscanf(input, "%s", cmd);

  char *offset = strchr(input, ' ');

  if (strcmp(cmd, "users")) {
    // msg type P_TYPE_ADMN_USERS
  } else if (strcmp(cmd, "remove")) {
    // msg type P_TYPE_ADMN_RMUSR;
  } else if (strcmp(cmd, "topics")) {
    // msg type P_TYPE_ADMN_TOPIC;
  } else if (strcmp(cmd, "show")) {
    // msg type P_TYPE_ADMN_SHWTOPIC;
  } else if (strcmp(cmd, "lock")) {
    // msg type P_TYPE_ADMN_LOCK;
  } else if (strcmp(cmd, "unlock")) {
    // msg type P_TYPE_ADMN_UNLOCK;
  } else if (strcmp(cmd, "close")) {
    // msg type P_TYPE_ADMN_CLOSE;
  } else {
    printf("Comando não reconhecido");
  }
}
