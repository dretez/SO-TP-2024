#include "../../include/communication.h"
#include <stdio.h>

// Limites predefinidos
#define MAX_USERS 10
#define MAX_TOPICS 20
#define MAX_PERSISTENT_MSGS 5

int main(int argc, char *argv[]) {

  if (access(FIFO_SRV, F_OK) == 0) {
    printf("[ERRO] Ja existe um servidor!\n");
    exit(3);
  }

  /****************************** ABRE SERVIDOR ******************************/

  mkfifo(FIFO_SRV, 0600);
  int fd = open(FIFO_SRV, O_RDWR);

  /******************************* INICIA DADOS *******************************/

  uint8_t userCount = 0;
  pid_t usersPid[MAX_USERS];
  for (short i = 0; i < MAX_USERS; i++)
    usersPid[i] = -1;

  uint8_t topicCount = 0;
  char topics[MAX_TOPICS][TAM_NOME_TOPICO];
  memset(topics, '\0', sizeof(topics));

  pid_t topicSubs[MAX_TOPICS];

  char *persistMSGs[MAX_TOPICS][MAX_PERSISTENT_MSGS];

  /* Dado o tamanho de um packet (~64KB) é preferível alocar um packet no heap
   * ao invés da stack de forma a evitar um stack overflow.
   * Outra solução seria diminuir o tamanho do buffer num packet, no entanto,
   * dado que um tamanho máximo para os usernames ainda não foi definido, é
   * impossível saber o quão grande um packet precisa de ser. */
  packet *p = (packet *)malloc(sizeof(packet));
  if (p == NULL) {
    printf("[ERRO] Falha ao iniciar pacote de dados");
    exit(3);
  }

  while (1) {

    if (0 /*recetor de pacotes?*/) {
      /**************************** AGUARDA PACOTE ****************************/

      int res = read(fd, &p->head, sizeof(packetHeader));
      res += read(fd, &p->buf, p->head.tam_msg);

      /*************************** VERIFICA PACOTE ***************************/

      if (res == sizeof(packetHeader) + p->head.tam_msg) {

        // código
      }

      /*************************** PROCESSA PACOTE ***************************/

      // código
    }

    if (0 /*emissor de mensagens?*/) {
      char fifo_cli[30];
      sprintf(fifo_cli, FIFO_CLI, p->head.pid);
      int fd_cli = open(fifo_cli, O_WRONLY);

      // prepara pacote a ser enviado

      int res = write(fd_cli, p, packetSize(*p));
      close(fd_cli);
    }
  }

  free(p);
  close(fd);
  unlink(FIFO_SRV);
  printf("FIM \n");
  exit(0);
}
