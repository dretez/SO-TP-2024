#include "../headers/manager.h"
#include "../../include/communication.h"
#include <string.h>

int main(int argc, char *argv[]) {

  if (access(FIFO_SRV, F_OK) == 0) {
    printf("[ERRO] Ja existe um servidor!\n");
    exit(3);
  }

  /****************************** ABRE SERVIDOR ******************************/

  mkfifo(FIFO_SRV, 0600);
  int fd = open(FIFO_SRV, O_RDWR);

  /******************************* INICIA DADOS *******************************/

  short userCount = 0;
  pid_t usersPid[MAX_USERS];
  for (short i = 0; i < MAX_USERS; i++)
    usersPid[i] = -1;
  char *usernames[MAX_USERS];
  for (short i = 0; i < MAX_USERS; i++)
    usernames[i] = NULL;

  short topicCount = 0;
  char topics[MAX_TOPICS][TAM_NOME_TOPICO + 1];
  memset(topics, '\0', sizeof(topics));

  pid_t topicSubs[MAX_TOPICS][MAX_USERS];
  for (short i = 0; i < MAX_TOPICS; i++) {
    for (short j = 0; j < MAX_TOPICS; j++) {
      topicSubs[i][j] = -1;
    }
  }
  unsigned short nTopicSubs[MAX_TOPICS];
  for (short i = 0; i < MAX_TOPICS; i++) {
    nTopicSubs[i] = 0;
  }

  persistMsg persistMSGs[MAX_TOPICS][MAX_PERSISTENT_MSGS];
  for (short i = 0; i < MAX_TOPICS * MAX_PERSISTENT_MSGS; i++) {
    persistMSGs[i]->lifetime = 0;
    persistMSGs[i]->msg = NULL;
  }

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
      case 0: // Pedido de lista de tópicos
        p->head.tipo_msg = 14;
        int offset;
        for (int i = 0, offset = 0; i < topicCount;
             i++, offset += strlen(topics[i]) + 1)
          strcpy(&p->buf[offset], topics[i]);
        p->head.tam_msg = offset;
        break;

      case 1: // Recebida mensagem para um tópico
        // muda o tipo de msg para 12
        // encontra o nome do tópico no buffer:
        /* para encontrar o tópico, devemos fazer as seguintes deslocações a
         * partir do ínicio do buffer: o tamanho de 1 int (4 bytes) (tempo de
         * vida da msg) o tamanho de 1 int + o valor indicado por esse int (nome
         * e tam) o tamanho de 1 int (indicador de tamanho do tópico) o último
         * int indica o tamanho do tópico, pelo que devemos copiar apenas o
         * número de chars indicados por esse int para um array que armazene o
         * nome do tópico
         * */
        // redireciona mensagem a todos os utilizadores que estejam subscritos
        // nesse tópico
        break;

      case 2: // Pedido de subscrição a um tópico
          ;
        int i;
        for (i = 0; i < topicCount; i++) {
          if (!strcmp(topics[i], p->buf /*tópico*/) &&
              nTopicSubs[i] < MAX_USERS) {
            topicSubs[i][nTopicSubs[i]++] = p->head.pid;
            break;
          }
        }
        if (i == topicCount && topicCount < MAX_TOPICS) {
          strcpy(topics[i], p->buf);
          topicSubs[i][nTopicSubs[i]++] = p->head.pid;
        }
        p->head.tipo_msg = 18;
        p->head.tam_msg = 0;
        break;

      case 3: // Pedido de remoção de subscrição a um tópico
        // recebe nome do tópico do p->buf
        // remove utilizador desse tópico
        // se não houverem utilizadores ou mensagens persistentes no tópico,
        // apaga-o envia confirmação de remoção de subscrição
        p->head.tipo_msg = 18;
        p->head.tam_msg = 0;
        break;

      case 4: // Pedido de saída de um cliente feed
        // remove utilizador da lista de utilizadores connectados
        // envia informação a todos os utilizadores que o utilizador saiu
        break;

      default: // Tipo de mensagem desconhecido
        p->head.tipo_msg = 17;
        p->head.tam_msg = sizeof(int);
        int errorMsg = 0;
        memcpy(p->buf, &errorMsg, sizeof(errorMsg));
        break;
      }

      // envia resposta para feeds
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
