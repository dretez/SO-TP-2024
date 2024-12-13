#include "../headers/dadosThreads.h"

int processCmd(packet *p, char *input);

void *admin_thread(void *args) {
  TDATA *td = (TDATA *)args;
  packet *p = td->p;
  int *fd = td->fifo_srv;

  /***************************** MODIFICA SINAIS *****************************/

  sigset_t sigs;
  sigemptyset(&sigs);
  sigaddset(&sigs, SIGINT);
  sigprocmask(SIG_UNBLOCK, &sigs, NULL);

  struct sigaction sa;

  sa.sa_flags = SA_SIGINFO;
  sa.sa_sigaction = stopManager;
  sigaction(SIGINT, &sa, NULL);

  /****************************************************************************/

  while (*td->cont) {
    printf(">>>");
    fflush(stdout);

    char cmdbuf[TAM_CMD_INPUT];
    fgets(cmdbuf, TAM_CMD_INPUT, stdin);
    cmdbuf[strlen(cmdbuf) - 1] = '\0';

    switch (processCmd(p, cmdbuf)) {
    case 0: {
      p->head.pid = getpid();
      write(*fd, p, packetSize(*p));
      break;
    }
    case 2: {
      p->head.pid = getpid();
      write(*fd, p, packetSize(*p));
      pthread_exit(NULL);
    }
    default:
      break;
    }
  }

  pthread_exit(NULL);
}

int processCmd(packet *p, char *input) {
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
    writeSingleValPacket(p, P_TYPE_ADMN_SHWTOPIC, &input[offset],
                         wordlen(&input[offset]) + 1);

  } else if (!strcmp(cmd, "lock")) {
    offset = nextword(input, offset, TAM_CMD_INPUT);
    writeSingleValPacket(p, P_TYPE_ADMN_LOCK, &input[offset],
                         wordlen(&input[offset]) + 1);

  } else if (!strcmp(cmd, "unlock")) {
    offset = nextword(input, offset, TAM_CMD_INPUT);
    writeSingleValPacket(p, P_TYPE_ADMN_UNLOCK, &input[offset],
                         wordlen(&input[offset]) + 1);

  } else if (!strcmp(cmd, "close")) {
    writeEmptyPacket(p, P_TYPE_ADMN_CLOSE);
    return 2;

  } else {
    printf("Comando não reconhecido");
    return 1;
  }
  return 0;
}

void stopManager() {
  int fd = open(FIFO_SRV, O_WRONLY);

  packet *p = (packet *)malloc(sizeof(packet));
  if (p == NULL) {
    printf("[ERRO] Não foi possível terminar o programa\n");
    return;
  }

  processCmd(p, "close\0");

  p->head.pid = getpid();
  write(fd, p, packetSize(*p));
  pthread_exit(NULL);
}
