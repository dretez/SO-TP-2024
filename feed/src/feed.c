#include "../../include/communication.h"
#include "../../include/globals.h"
#include "../headers/input.h"

int main(int argc, char *argv[]) {

  /****************** VERIFICAÇÃO E TRATAMENTO DE ARGUMENTOS ******************/

  if (argc != 2) {
    printf("[SINTAXE] Numero de argumentos incorreto\n");
    exit(1);
  }

  char *nome = argv[1];

  /************ INICIA PROCESSOS PARA RECEÇÃO E ENVIO DE MENSAGEMS ************/
  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   * Dada a necessidade de poder receber mensagens a qualquer momento, mesmo
   * enquanto o utilizador está a escrever uma mensagem, poderá ser necessário
   * fazer aqui um fork() de forma a criar um programa que trate do envio de
   * mensagens e outro que trate da receção
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

  if (access(FIFO_SRV, F_OK) != 0) {
    printf("[ERRO] Servidor nao esta a correr!\n");
    exit(3);
  }

  char *fifo_cli;
  sprintf(fifo_cli, FIFO_CLI, getpid());
  mkfifo(fifo_cli, 0600);

  int fd_cli, fd;
  fd_cli = open(fifo_cli, O_RDWR);
  fd = open(FIFO_SRV, O_WRONLY);

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

  /******************** INDICAR EXISTÊNCIA DO COMANDO HELP ********************/
  // TODO:

  /****************** REALIZA O ENVIO E RECEÇÃO DE MENSAGEMS ******************/
  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   * A seguinte secção controla o envio e a receção de mensagens. Para manter
   * o código mais legível e organizado, será boa ideia escrever o código
   * para cada uma destas funções em ficheiros separados
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

  if (0 /*recetor de mensagens?*/) {
    while (1) {
      /*************************** AGUARDA MENSAGEM ***************************/

      int res = read(fd_cli, &p->head, sizeof(packetHeader));
      res = read(fd_cli, &p->buf, p->head.tam_msg);

      /********************* VERIFICA TÓPICO DE MENSAGEM *********************/

      char topico[TAM_NOME_TOPICO];

      switch (p->head.tipo_msg ) {
        case 0: // Listagem de tópicos (topics)
          printf("Tópicos disponíveis:\n");
        printf("%s\n", p->buf);
        break;

        case 1: // Mensagem recebida de um tópico subscrito
          printf("Nova mensagem no tópico '%s' : %s\n",
                 topico, p->buf);
        break;

        case 2: // Confirmação de subscrição
          printf("Subscreveu o tópico: %s\n", topico);
        break;

        case 3: // Confirmação de cancelamento de subscrição
          printf("Cancelou a subscrição do tópico: %s\n", topico);
        break;

        case 4: // Pedido de saída (exit)
          printf(" Enviando o pedido para sair.");
          printf("A sair...\n");
        close(fd_cli);
        unlink(fifo_cli);
        exit(0);
        break;

        default: // Tipo de mensagem desconhecido
          printf("[ERRO] Tipo de mensagem desconhecido: %d\n", p->head.tipo_msg);
        break;
      }
      /*************************** MOSTRA MENSAGEM ***************************/

      // código
    }
  }

  if (0 /*emissor de mensagens?*/) {
    char cmdbuf[TAM_CMD_INPUT];
    while (1) {
      /************** AGUARDA INTRODUÇÃO DE MENSAGEM OU COMANDO **************/

      fgets(cmdbuf, TAM_CMD_INPUT, stdin);

      /******************** PROCESSA COMANDOS E MENSAGENS ********************/

      char cmd[12];
      sscanf(cmdbuf, "%s", cmd);

      char *offset = strchr(cmdbuf, ' ');

      if (strcmp(cmd, "topics")) {
        writeEmptyPacket(p, 0);
      } else if (strcmp(cmd, "exit")) {
        writeEmptyPacket(p, 4);
      } else if (strcmp(cmd, "subscribe")) {
        char topic[TAM_NOME_TOPICO];
        sscanf(offset + 1, "%s", topic);
        writeSingleValPacket(p, 2, topic, strlen(topic));
      } else if (strcmp(cmd, "unsubscribe")) {
        char topic[TAM_NOME_TOPICO];
        sscanf(offset + 1, "%s", topic);
        writeSingleValPacket(p, 3, topic, strlen(topic));
      } else if (strcmp(cmd, "msg")) {
        char topic[TAM_NOME_TOPICO];
        int i;
        for (i = 0; i < TAM_NOME_TOPICO || offset[i + 1] == ' '; i++)
          topic[i] = offset[i + 1];
        topic[i] = '\0';

        offset = strchr(offset + 1, ' ');
        int duracao;
        sscanf(offset + 1, "%d", &duracao);

        offset = strchr(offset + 1, ' ');
        char msg[TAM_CORPO_MSG];
        sscanf(offset + 1, "%s", msg);

        writeMsgPacket(p, 1, duracao, topic, argv[1], msg);
      } else if (strcmp(cmd, "help")) {
        printf("\ntopics - Mostrar uma lista com todos os tópicos ");
        printf("\nmsg <topico> <duração> <mensagem> - Enviar mensagem para um determinado tópico");
        printf("\nsubscribe <nome do tópico> - Subscrever um determinado tópico");
        printf("\nunsubscribe <nome do topico> - Deixar de subscrever um determinado tópico");
        printf("\nexit - Sair\n ");
      } else {
        printf("Comando não reconhecido");
      }

      /**************************** ENVIA MENSAGEM ****************************/

      p->head.pid = getpid();
      int res = write(fd, p, packetSize(*p));
    }
  }

  /***************************** TERMINA PROGRAMA *****************************/
  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   * Caso um dos processos (recetor ou emissor) tenha terminado, ambos os
   * processos são terminados.
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
  printf("FIM! \n");
  close(fd);
  close(fd_cli);
  unlink(fifo_cli);
  exit(0);
}
