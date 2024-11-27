#include "../headers/manager.h"
#include "../headers/dados.h"

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

    if (0 /*recetor de pacotes?*/) {
      /**************************** AGUARDA PACOTE ****************************/

      int res = read(fd, &p->head, sizeof(packetHeader));
      res += read(fd, &p->buf, p->head.tam_msg);

      /*************************** VERIFICA PACOTE ***************************/

      if (res == sizeof(packetHeader) + p->head.tam_msg) {

        // código
      }

      /*************************** PROCESSA PACOTE ***************************/

      switch (p->head.tipo_msg) {
      case P_TYPE_USER_TOPIC: // Pedido de lista de tópicos
      {
        p->head.tipo_msg = P_TYPE_MNGR_TOPIC;
        unsigned short offset;
        for (int i = 0, offset = 0; i < d.ntopics;
             offset += strlen(d.topics[i++].name) + 1)
          strcpy(&p->buf[offset], d.topics[i].name);
        p->head.tam_msg = offset;
        break;
      }
      case P_TYPE_USER_MSG: { // Recebida mensagem para um tópico
        p->head.tipo_msg = P_TYPE_MNGR_MSG;
        break;
      }
      case P_TYPE_USER_SUB: { // Pedido de subscrição a um tópico
        topic *topico = getTopic(d.topics, d.ntopics, p->buf);
        if (topico == NULL && addTopic(&d, p->buf) == 1) {
          writeErrorPacket(p, P_ERR_TOPIC_LIST_FULL);
          break;
        }
        if (subscribeUser(topico, p->head.pid)) {
          writeErrorPacket(p, P_ERR_ALREADY_SUBBED);
          break;
        }
        p->head.tipo_msg = P_TYPE_MNGR_SUCCESS;
        p->head.tam_msg = 0;
        break;
      }
      case P_TYPE_USER_UNSUB: { // Pedido de remoção de subscrição a um tópico
        topic *topico = getTopic(d.topics, d.ntopics, p->buf);
        if (topico == NULL) {
          writeErrorPacket(p, P_ERR_INVALID_TOPIC);
          break;
        }
        if (unsubscribeUser(topico, p->head.pid)) {
          writeErrorPacket(p, P_ERR_NOT_SUBBED);
          break;
        }
        p->head.tipo_msg = P_TYPE_MNGR_SUCCESS;
        p->head.tam_msg = 0;
        break;
      }
      case P_TYPE_USER_EXIT: // Pedido de saída de um cliente feed
        // remove utilizador da lista de utilizadores connectados
        // envia informação a todos os utilizadores que o utilizador saiu
        break;

      default: // Tipo de mensagem desconhecido
        writeErrorPacket(p, P_ERR_GENERIC);
        break;
      }

      /************************ ENVIA PACOTE AOS FEEDS ************************/

      switch (p->head.tipo_msg) {
      case P_TYPE_MNGR_MSG: {
        int offset = sizeof(int);
        offset += strlen(&p->buf[offset]) + 1;
        char topico[TAM_NOME_TOPICO];
        strcpy(topico, &p->buf[offset]);
        if (getTopic(d.topics, d.ntopics, topico) == NULL)
          addTopic(&d, topico);
        topic *t = getTopic(d.topics, d.ntopics, topico);
        for (int i = 0; i < t->nsubs; i++) {
          char fifo_cli[30];
          sprintf(fifo_cli, FIFO_CLI, t->subs[i]);
          int fd_cli = open(fifo_cli, O_WRONLY);

          int res = write(fd_cli, p, packetSize(*p));

          close(fd_cli);
        }
      }
      case P_TYPE_MNGR_RMUSR: {
        // envia a todos os utilizadores ligados
        // d.users contém a lista de utilizadores
      }
      default: {
        char fifo_cli[30];
        sprintf(fifo_cli, FIFO_CLI, p->head.pid);
        int fd_cli = open(fifo_cli, O_WRONLY);

        int res = write(fd_cli, p, packetSize(*p));

        close(fd_cli);
      }
      }
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
