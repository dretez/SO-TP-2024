#include "../headers/manager.h"
#include "../headers/dados.h"
#include "../headers/recetor.h"

int main(int argc, char *argv[]) {

  if (access(FIFO_SRV, F_OK) == 0) {
    printf("[ERRO] Ja existe um servidor!\n");
    exit(3);
  }

  /****************************** ABRE SERVIDOR ******************************/

  mkfifo(FIFO_SRV, 0600);
  int fd = open(FIFO_SRV, O_RDWR);

  /******************************* INICIA DADOS *******************************/

  managerData d;
  initManData(&d);

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

    if (0 /*servidor?*/) {
      /**************************** AGUARDA PACOTE ****************************/
      read(fd, &p->head, sizeof(packetHeader));
      read(fd, &p->buf, p->head.tam_msg);

      /*************************** PROCESSA PACOTE ***************************/
      if (processPacket(p, &d))
        // TODO: maybe send a message to all the feeds informing the server's
        // closing
        goto exit;

      /************************ ENVIA PACOTE AOS FEEDS ************************/
      answer(p, &d);
    }

    if (0 /*administrador?*/) {
      char fifo_cli[30];
      sprintf(fifo_cli, FIFO_CLI, p->head.pid);
      int fd_cli = open(fifo_cli, O_WRONLY);

      // prepara pacote a ser enviado

      int res = write(fd_cli, p, packetSize(*p));
      close(fd_cli);
    }
  }

exit:
  // TODO: clean the managerData struct before exiting, as this struct contains
  // allocated memory that needs to be free()'d
  free(p);
  close(fd);
  unlink(FIFO_SRV);
  printf("FIM \n");
  exit(0);
}
