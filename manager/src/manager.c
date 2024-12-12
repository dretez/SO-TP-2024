#include "../headers/manager.h"

int main() {

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
  if (p_admn == NULL) {
    printf("[ERRO] Falha ao iniciar pacote de dados");
    ext_err = 3;
    goto exit2;
  }

  packet *p_counter = (packet *)malloc(sizeof(packet));
  if (p_srv == NULL) {
    printf("[ERRO] Falha ao iniciar pacote de dados");
    ext_err = 3;
    goto exit3;
  }

  /***************************** DESATIVA SINAIS *****************************/

  sigset_t sigs;
  sigemptyset(&sigs);
  sigaddset(&sigs, SIGINT);
  sigprocmask(SIG_BLOCK, &sigs, NULL);

  /****************************** INICIA THREADS ******************************/

  int cont = 1;

  TDATA t[2];
  t[0].p = p_admn;
  t[0].cont = &cont;
  t[0].fifo_srv = &fd;
  t[1].p = p_counter;
  t[1].cont = &cont;
  t[1].fifo_srv = &fd;

  pthread_t th[2];

  pthread_create(&th[0], NULL, admin_thread, (void *)&t[0]);

  pthread_create(&th[1], NULL, counter_thread, (void *)&t[1]);

  /********************************* SERVIDOR *********************************/
  while (1) {
    read(fd, &p_srv->head, sizeof(packetHeader));
    read(fd, &p_srv->buf, p_srv->head.tam_msg);

    /*************************** PROCESSA PACOTE ***************************/
    processPacket(p_srv, &d);

    /************************ ENVIA PACOTE AOS FEEDS ************************/
    if (answer(p_srv, &d))
      goto exit;
  }

exit:
  cont = 0;
  pthread_join(th[0], NULL);
  pthread_join(th[1], NULL);
  char *savedata = getenv(SAVEFILE);
  if (savedata == NULL) {
    printf("\"%s\" não está definido, as mensagens não serão armazenadas.\n",
           SAVEFILE);
  } else {
    FILE *f = fopen(savedata, "wt");
    savePMsgs(d, f);
    fclose(f);
  }
  // TODO: clean the managerData struct before exiting, as this struct contains
  // allocated memory that needs to be free()'d
  free(p_counter);
exit3:
  free(p_admn);
exit2:
  free(p_srv);
exit1:
  close(fd);
  unlink(FIFO_SRV);
  exit(ext_err);
}
