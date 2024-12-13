#include "../headers/feed.h"

int main(int argc, char *argv[]) {

  /****************** VERIFICAÇÃO E TRATAMENTO DE ARGUMENTOS ******************/

  if (argc != 2) {
    printf("[SINTAXE] Número de argumentos incorreto\n"
           "Esperado: %s <username>\n",
           argv[0]);
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
  if (login(p, nome, fd, fd_cli))
    goto exit;

  /***************************** DESATIVA SINAIS *****************************/

  struct sigaction sa;

  sa.sa_flags = 0;
  sa.sa_handler = SIG_IGN;
  sigaction(SIGINT, &sa, NULL);

  /***************************** MODIFICA SINAIS *****************************/

  sa.sa_flags = SA_SIGINFO;
  sa.sa_sigaction = stopFeed;
  sigaction(SIGINT, &sa, NULL);

  /******************** INDICAR EXISTÊNCIA DO COMANDO HELP ********************/

  printf("Bem vindo! Utilize o comado \"help\" para obter uma lista de "
         "comandos\n");

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
        write(fd, p, packetSize(*p));
      }
    }
  }

  /***************************** TERMINA PROGRAMA *****************************/
exit:
  free(p);
  close(fd);
  close(fd_cli);
  unlink(fifo_cli);
  exit(0);
}

void stopFeed() {
  int fd = open(FIFO_SRV, O_WRONLY);

  packet *p = (packet *)malloc(sizeof(packet));
  if (p == NULL) {
    printf("[ERRO] Não foi possível terminar o programa\n");
    return;
  }

  processCmd(p, "exit\0", "\0");
  p->head.pid = getpid();
  write(fd, p, packetSize(*p));
}
