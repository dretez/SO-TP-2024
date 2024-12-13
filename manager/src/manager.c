#include "../headers/manager.h"
#include <pthread.h>

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

  /* Dado o tamanho de um packet (~64KB) é preferível alocar um packet no heap
   * ao invés da stack de forma a evitar um stack overflow.
   * Outra solução seria diminuir o tamanho do buffer num packet, no entanto,
   * dado que um tamanho máximo para os usernames ainda não foi definido, é
   * impossível saber o quão grande um packet precisa de ser. */
  packet *p[TCOUNT + 1];
  for (int i = 0; i < TCOUNT + 1; i++) {
    packet *pp = (packet *)malloc(sizeof(packet));
    if (pp == NULL) {
      printf("[ERRO] Falha ao iniciar pacote de dados");
      for (; i > 0; i--)
        free(p[i - 1]);
      close(fd);
      unlink(FIFO_SRV);
      exit(3);
    }
    p[i] = pp;
  }

  /***************************** DESATIVA SINAIS *****************************/

  sigset_t sigs;
  sigemptyset(&sigs);
  sigaddset(&sigs, SIGINT);
  sigprocmask(SIG_BLOCK, &sigs, NULL);

  /****************************** INICIA THREADS ******************************/

  int cont = 1;

  TDATA t[TCOUNT];
  pthread_t th[TCOUNT];
  /* Array de funções do tipo "void *" que recebem um argumento do tipo "void *"
   * Estas funções servem como ponto de início para as threads criadas */
  void *(*routines[TCOUNT])(void *) = {admin_thread, counter_thread};
  for (int i = 0; i < TCOUNT; i++) {
    t[i].p = p[i + 1];
    t[i].cont = &cont;
    t[i].fifo_srv = &fd;
    pthread_create(&th[i], NULL, routines[i], (void *)&t[i]);
  }

  /********************************* SERVIDOR *********************************/
  while (1) {
    read(fd, &p[0]->head, sizeof(packetHeader));
    read(fd, &p[0]->buf, p[0]->head.tam_msg);

    /*************************** PROCESSA PACOTE ***************************/
    processPacket(p[0], &d);

    /************************ ENVIA PACOTE AOS FEEDS ************************/
    if (answer(p[0], &d))
      break;
  }

  /****************************** LIMPEZA E FIM ******************************/

  cont = 0;
  for (int i = 0; i < TCOUNT; i++)
    pthread_join(th[i], NULL);
  char *savedata = getenv(SAVEFILE);
  if (savedata == NULL) {
    printf("\"%s\" não está definido, as mensagens não serão armazenadas.\n",
           SAVEFILE);
  } else {
    FILE *f = fopen(savedata, "w");
    savePMsgs(d, f);
    fclose(f);
  }
  clearManData(&d);
  for (int i = 0; i < TCOUNT; i++)
    free(p[i]);
  close(fd);
  unlink(FIFO_SRV);
  exit(0);
}
