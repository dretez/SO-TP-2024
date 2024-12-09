#include "../headers/feed.h"

int main(int argc, char *argv[]) {

  /****************** VERIFICAÇÃO E TRATAMENTO DE ARGUMENTOS ******************/

  if (argc != 2) {
    printf("[SINTAXE] Numero de argumentos incorreto\n");
    exit(1);
  }

  char *nome = argv[1];

  /************ INICIA PROCESSOS PARA RECEÇÃO E ENVIO DE MENSAGEMS ************/

  if (access(FIFO_SRV, F_OK) != 0) {
    printf("[ERRO] Servidor nao esta a correr!\n");
    exit(3);
  }
  int fd = open(FIFO_SRV, O_WRONLY);

  char fifo_cli[20];
  sprintf(fifo_cli, FIFO_CLI, getpid());
  mkfifo(fifo_cli, 0600);

  int fd_cli = open(fifo_cli, O_RDWR);

  /* Dado o tamanho de um packet (~64KB) é preferível alocar um packet no heap
   * ao invés da stack de forma a evitar um stack overflow.
   * Outra solução seria diminuir o tamanho do buffer num packet, no entanto,
   * dado que um tamanho máximo para os usernames ainda não foi definido, é
   * impossível saber o quão grande um packet precisa de ser. */
  packet *p = (packet *)malloc(sizeof(packet));
  if (p == NULL) {
    printf("[ERRO] Falha ao iniciar pacote de dados\n");
    exit(3);
  }

  printf("A fazer login...\n");
  writeSingleValPacket(p, P_TYPE_USER_HANDSHK, nome, strlen(nome) + 1);
  p->head.pid = getpid();
  write(fd, p, packetSize(*p));

  read(fd_cli, &p->head, sizeof(packetHeader));
  read(fd_cli, &p->buf, p->head.tam_msg);

  switch (p->head.tipo_msg) {
  case P_TYPE_MNGR_SUCCESS: {
    int scsCode;
    memcpy(&scsCode, p->buf, sizeof(int));
    if (scsCode == P_SCS_HANDSHK)
      break;
    printf(NOTIF_ERR, NOTIF_ERR_LOGIN);
    goto exit;
  }
  case P_TYPE_MNGR_ERROR: {
    int errCode;
    memcpy(&errCode, p->buf, sizeof(int));

    switch (errCode) {
    case P_ERR_ALREADY_LOGGED:
      printf(NOTIF_ERR, NOTIF_ALREADY_LOGGED);
      break;
    default:
      printf(NOTIF_ERR, NOTIF_ERR_LOGIN);
      break;
    }
    goto exit;
  }
  default:
    printf(NOTIF_ERR, NOTIF_ERR_LOGIN);
    goto exit;
  }

  /******************** INDICAR EXISTÊNCIA DO COMANDO HELP ********************/
  // TODO:

  /****************** REALIZA O ENVIO E RECEÇÃO DE MENSAGEMS ******************/

  while (1) {
    printf(">>>");
    fflush(stdout);

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    FD_SET(fd_cli, &fds);

    int n = select(fd_cli + 1, &fds, NULL, NULL, NULL);

    if (n <= 0)
      continue;

    if (FD_ISSET(fd_cli, &fds)) {
      read(fd_cli, &p->head, sizeof(packetHeader));
      read(fd_cli, &p->buf, p->head.tam_msg);

      if (processPacket(p))
        goto exit;
    }

    if (FD_ISSET(0, &fds)) {
      char cmdbuf[TAM_CMD_INPUT];
      fgets(cmdbuf, TAM_CMD_INPUT, stdin);
      cmdbuf[strlen(cmdbuf) - 1] = '\0';

      if (!processCmd(p, cmdbuf, nome)) {
        p->head.pid = getpid();
        int res = write(fd, p, packetSize(*p));
      }
    }
  }

  /***************************** TERMINA PROGRAMA *****************************/
exit:
  printf("FIM! \n");
  free(p);
  close(fd);
  close(fd_cli);
  unlink(fifo_cli);
  exit(0);
}
