#include "../headers/input.h"

void inputcmd(char *cmdbuf, int size) {
  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   * Seguem-se duas formas de começar esta função. O fgets garante mais
   * segurança e mais controlo sobre os dados recebidos, enquanto que o scanf
   * geralmente permite ter código mais curto.
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

  // scanf("%s", cmdbuf);

  // fgets(cmdbuf, size, stdin);

  /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   * Existem 3 tipos de comandos que queremos poder receber:
   *
   *  - <cmd>
   *    Um comando sem mais argumentos.
   *
   *  - <cmd> <string>
   *    Um comando que recebe como argumento um array de chars. Usado pelos
   *    comandos subscribe, unsubscribe, lock e unlock para receber o nome de
   *    um tópico, e pelo comando remove para receber o nome de um utilizador.
   *
   *  - <cmd> <string> <int> <string>
   *    A estrutura do comando msg. Recebe, por esta sequência: um array de
   *    chars sem espaços para o nome do tópico, um array de chars que deve
   *    ser convertido para um int para o tempo de vida da mensagem e um array
   *    de chars que pode possuir espaços para a mensagem a ser enviada.
   *
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   *
   *  Segue-se uma cópia da lista de comandos sem toda a informação extra
   *  presente no "communication.h":
   *
   ************************* COMANDOS DE UTILIZADOR ***************************
   *
   *  - topics
   *  - msg <tópico> <duração> <mensagem>
   *  - subscribe <tópico>
   *  - unsubscribe <tópico>
   *  - exit
   *
   ************************* COMANDOS DE ADMINISTRADOR *************************
   *
   *  - users
   *  - remove <username>
   *  - topics
   *  - show <tópico>
   *  - lock <tópico>
   *  - unlock <tópico>
   *  - close
   *
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}
