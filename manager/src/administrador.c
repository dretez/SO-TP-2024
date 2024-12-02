#include "../headers/dadosThreads.h"

void processCmd(packet *p, char *input, char *username);

void *admin_thread(void *args) {
  TDATA *td = (TDATA *)args;
  packet *p = td->p;
  int *fd = td->fifo_srv;

  while (td->cont) {
    printf(">>>");
    fflush(stdout);

    char cmdbuf[TAM_CMD_INPUT];
    fgets(cmdbuf, TAM_CMD_INPUT, stdin);

    processCmd(p, cmdbuf, "admin");

    p->head.pid = getpid();
    int res = write(*fd, p, packetSize(*p));
  }

  pthread_exit(NULL);
}

void processCmd(packet *p, char *input, char *username) {
  char cmd[12];
  sscanf(input, "%s", cmd);

  char *offset = strchr(input, ' ') + 1;

  if (strcmp(cmd, "users")) {
    writeEmptyPacket(p, P_TYPE_ADMN_USERS);

  } else if (strcmp(cmd, "remove")) {
    writeSingleValPacket(p, P_TYPE_ADMN_RMUSR, offset, strlen(offset));

  } else if (strcmp(cmd, "topics")) {
    writeEmptyPacket(p, P_TYPE_ADMN_TOPIC);

  } else if (strcmp(cmd, "show")) {
    writeSingleValPacket(p, P_TYPE_ADMN_SHWTOPIC, offset, strlen(offset));

  } else if (strcmp(cmd, "lock")) {
    writeSingleValPacket(p, P_TYPE_ADMN_LOCK, offset, strlen(offset));

  } else if (strcmp(cmd, "unlock")) {
    writeSingleValPacket(p, P_TYPE_ADMN_UNLOCK, offset, strlen(offset));

  } else if (strcmp(cmd, "close")) {
    writeEmptyPacket(p, P_TYPE_ADMN_CLOSE);

  } else {
    printf("Comando não reconhecido");
  }
}
