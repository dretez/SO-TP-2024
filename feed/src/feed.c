#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

  /****************** VERIFICAÇÃO E TRATAMENTO DE ARGUMENTOS ******************/

  if (argc != 2 ){
printf("Numero de argumentos incorreto\n");
return 1;
}

 char *nome = argv[1];

  /************ INICIA PROCESSOS PARA RECEÇÃO E ENVIO DE MENSAGEMS ************/
  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   * Dada a necessidade de poder receber mensagens a qualquer momento, mesmo
   * enquanto o utilizador está a escrever uma mensagem, poderá ser necessário
   * fazer aqui um fork() de forma a criar um programa que trate do envio de
   * mensagens e outro que trate da receção
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

if(access ( FIFO_SRV, F_OK)!=0){
printf("[ERRO] Servidor nao esta a correr!\n");
exit(3);
}
sprintf (fifo_cli,  FIFO_CLI, getpid());
mkfifo( fifo_cli , 0600);
fd_cli = open (fifo_cli,O_RDWR);
fd = open ( FIFO_SRV, O_WRONLY);

  /****************** REALIZA O ENVIO E RECEÇÃO DE MENSAGEMS ******************/
  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   * A seguinte secção controla o envio e a receção de mensagens. Para manter
   * o código mais legível e organizado, será boa ideia escrever o código
   * para cada uma destas funções em ficheiros separados
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
  do{
scanf("%s", str);
p.pid = getpid();
strcpy(p.str, str);
res  = write ( fd, &p , sizeof (PEDIDO));
if ( res == sizeof(PEDIDO)){
printf("ENVIEI  ... '%s' - %d ( %d bytes) \n", p.str,p.pid,res);
res = read (fd_cli, &r, sizeof(RESPOSTA));
printf("RECEBI... '%s' (%d)\n", r.str, res);
}
}while ( strcmp(str,"fim")!=0 && strcmp(str, "quit")!=0);
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
  printf("FIM! \n");
close(fd);
close(fd_cli);
unlink (fifo_cli);
  exit(0);
}
