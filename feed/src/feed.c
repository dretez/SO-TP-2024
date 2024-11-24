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

  /****************** REALIZA O ENVIO E RECEÇÃO DE MENSAGEMS ******************/
  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   * A seguinte secção controla o envio e a receção de mensagens. Para manter
   * o código mais legível e organizado, será boa ideia escrever o código
   * para cada uma destas funções em ficheiros separados
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

  if (0 /*recetor de mensagens?*/) {
    packet p;
    while (1) {
      /*************************** AGUARDA MENSAGEM ***************************/

      int res = read(fd_cli, &p.head, sizeof(packetHeader));
      res = read(fd_cli, &p.buf, p.head.tam_msg);

      /********************* VERIFICA TÓPICO DE MENSAGEM *********************/

      // código

      /*************************** MOSTRA MENSAGEM ***************************/

      // código
    }
  }

  if (0 /*emissor de mensagens?*/) {
    char cmd[TAM_CMD_INPUT];
    packet p;
    while (1) {
      /************** AGUARDA INTRODUÇÃO DE MENSAGEM OU COMANDO **************/

      inputcmd(cmd, TAM_CMD_INPUT);

      /******************** PROCESSA COMANDOS E MENSAGENS ********************/

      // código

      /**************************** ENVIA MENSAGEM ****************************/

      p.head.pid = getpid();
      int res = write(fd, &p, packetSize(p));
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
