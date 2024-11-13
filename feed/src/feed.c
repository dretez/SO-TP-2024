#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

  /****************** VERIFICAÇÃO E TRATAMENTO DE ARGUMENTOS ******************/

  // código...

  /************ INICIA PROCESSOS PARA RECEÇÃO E ENVIO DE MENSAGEMS ************/
  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   * Dada a necessidade de poder receber mensagens a qualquer momento, mesmo
   * enquanto o utilizador está a escrever uma mensagem, poderá ser necessário
   * fazer aqui um fork() de forma a criar um programa que trate do envio de
   * mensagens e outro que trate da receção
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

  // código...

  /****************** REALIZA O ENVIO E RECEÇÃO DE MENSAGEMS ******************/
  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   * A seguinte secção controla o envio e a receção de mensagens. Para manter
   * o código mais legível e organizado, será boa ideia escrever o código
   * para cada uma destas funções em ficheiros separados
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
  if (0 /*recetor de mensagens?*/) {
    while (1) {
      /*************************** AGUARDA MENSAGEM ***************************/

      // código...

      /********************* VERIFICA TÓPICO DE MENSAGEM *********************/

      // código

      /*************************** MOSTRA MENSAGEM ***************************/

      // código
    }
  }

  if (0 /*emissor de mensagens?*/) {
    while (1) {
      /************** AGUARDA INTRODUÇÃO DE MENSAGEM OU COMANDO **************/

      // código

      /******************** PROCESSA COMANDOS E MENSAGENS ********************/

      // código

      /**************************** ENVIA MENSAGEM ****************************/

      // código
    }
  }

  /***************************** TERMINA PROGRAMA *****************************/
  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   * Caso um dos processos (recetor ou emissor) tenha terminado, ambos os
   * processos são terminados.
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
  exit(0);
}
