#include "../headers/dadosThreads.h"

void processCmd(packet *p, char *input);

void *admin_thread(void *args) {
  TDATA *td = (TDATA *)args;
  packet *p = td->p;
  int *fd = td->fifo_srv;

  while (td->cont) {
    printf(">>>");
    fflush(stdout);

    char cmdbuf[TAM_CMD_INPUT];
    fgets(cmdbuf, TAM_CMD_INPUT, stdin);
    cmdbuf[strlen(cmdbuf) - 1] = '\0';

    processCmd(p, cmdbuf);

    p->head.pid = getpid();
    int res = write(*fd, p, packetSize(*p));
  }

  pthread_exit(NULL);
}

void processCmd(packet *p, char *input) {
  char cmd[12];
  int offset = 0;
  if (input[0] == ' ') {
    offset = nextword(input, 0, TAM_CMD_INPUT);
    offset = offset == -1 ? 0 : offset;
    wordncpy(cmd, &input[offset], 12);
  } else {
    wordncpy(cmd, input, 12);
  }

  if (!strcmp(cmd, "users")) {
    writeEmptyPacket(p, P_TYPE_ADMN_USERS);

  } else if (!strcmp(cmd, "remove")) {
    offset = nextword(input, offset, TAM_CMD_INPUT);
    writeSingleValPacket(p, P_TYPE_ADMN_RMUSR, &input[offset],
                         wordlen(&input[offset]));

  } else if (!strcmp(cmd, "topics")) {
    writeEmptyPacket(p, P_TYPE_ADMN_TOPIC);

  } else if (!strcmp(cmd, "show")) {
    offset = nextword(input, offset, TAM_CMD_INPUT);
    writeSingleValPacket(p, P_TYPE_ADMN_RMUSR, &input[offset],
                         wordlen(&input[offset]));

  } else if (!strcmp(cmd, "lock")) {
    offset = nextword(input, offset, TAM_CMD_INPUT);
    writeSingleValPacket(p, P_TYPE_ADMN_RMUSR, &input[offset],
                         wordlen(&input[offset]));

  } else if (!strcmp(cmd, "unlock")) {
    offset = nextword(input, offset, TAM_CMD_INPUT);
    writeSingleValPacket(p, P_TYPE_ADMN_RMUSR, &input[offset],
                         wordlen(&input[offset]));

  } else if (!strcmp(cmd, "close")) {
    writeEmptyPacket(p, P_TYPE_ADMN_CLOSE);

  } else {
    printf("Comando não reconhecido");
  }
}
