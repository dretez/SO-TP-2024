#include "../headers/manager.h"

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

  int ext_err = 0;

  /* Dado o tamanho de um packet (~64KB) é preferível alocar um packet no heap
   * ao invés da stack de forma a evitar um stack overflow.
   * Outra solução seria diminuir o tamanho do buffer num packet, no entanto,
   * dado que um tamanho máximo para os usernames ainda não foi definido, é
   * impossível saber o quão grande um packet precisa de ser. */
  packet *p_srv = (packet *)malloc(sizeof(packet));
  if (p_srv == NULL) {
    printf("[ERRO] Falha ao iniciar pacote de dados");
    ext_err = 3;
    goto exit1;
  }

  packet *p_admn = (packet *)malloc(sizeof(packet));
  if (p_srv == NULL) {
    printf("[ERRO] Falha ao iniciar pacote de dados");
    ext_err = 3;
    goto exit2;
  }

  int cont = 1;

  TDATA t;
  t.p = p_admn;
  t.cont = &cont;

  pthread_t th[2];

  pthread_create(&th[0], NULL, admin_thread, (void *)&t);

  pthread_create(&th[1], NULL, counter_thread, (void *)&cont);

  /********************************* SERVIDOR *********************************/
  while (1) {
    /**************************** AGUARDA PACOTE ****************************/
    read(fd, &p_srv->head, sizeof(packetHeader));
    read(fd, &p_srv->buf, p_srv->head.tam_msg);

    /*************************** PROCESSA PACOTE ***************************/
    if (processPacket(p_srv, &d))
      // TODO: maybe send a message to all the feeds informing the server's
      // closing
      goto exit;

    /************************ ENVIA PACOTE AOS FEEDS ************************/
    answer(p_srv, &d);
  }

exit:
  cont = 0;
  pthread_join(th[0], NULL);
  pthread_join(th[1], NULL);
  // TODO: clean the managerData struct before exiting, as this struct contains
  // allocated memory that needs to be free()'d
exit2:
  free(p_srv);
exit1:
  close(fd);
  unlink(FIFO_SRV);
  exit(ext_err);
}
