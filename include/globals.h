// Includes
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// pipes
#define FIFO_SRV "srv_pipe"
#define FIFO_CLI "cli_%d"

// Dados mensagens
#define TAM_NOME_TOPICO 20
#define TAM_CORPO_MSG 300

// Macros
#define maxval(type)                                                           \
  (1 << sizeof(type) * CHAR_BIT) - 1 // obtém o valor maximo permitido por type
                                     // ou o equivalente a colocar todos os
                                     // bits de type a 1

typedef struct {
  short tipo_msg, tam_msg;
  char buf[maxval(short)];
} msg;

/*
 ************************ TIPOS DE MENSAGENS / COMANDOS ************************
 *
 *************************** COMANDOS DE UTILIZADOR ***************************
 *
 *    - topics
 *      * obter lista de tópicos
 *      * tipo mensagem 0
 *      * tamanho 0
 *      * buf vazio
 *    - msg <tópico> <duração> <mensagem>
 *      * enviar mensagem para um tópico
 *      * tipo mensagem 1
 *      * tamanho variável
 *        * tamanho indicadores de tamanho (3 * 4 = 12) +
 *        * tamanho nome +
 *        * tamanho int (tempo de vida) (4) +
 *        * tamanho tópico (max 20) +
 *        * tamanho mensagem (max 300)
 *      * buf:
 *        * tempo de vida
 *        * tamanho nome
 *        * nome
 *        * tamanho tópico
 *        * tópico
 *        * tamanho mensagem
 *        * mensagem
 *    - subscribe / unsubscribe
 *      * subscrever / desubscrever , criação / destruição de tópicos
 *      * tipos mensagem 2 / 3
 *      * tamanho:
 *        * tamanho tópico
 *      * buf:
 *        * tópico
 *    - exit
 *      * terminar sessão do utilizador
 *      * tipo mensagem 4
 *      * tamanho 0
 *      * buf vazio
 *
 ************************** COMANDOS DE ADMINISTRADOR **************************
 *
 *    - users
 *      * obter lista de utilizadores ativos
 *      * tipo mensagem 5
 *      * tamanho 0
 *      * buf vazio
 *    - remove <username>
 *      * termina a sessão de um utilizador
 *      * tipo mensagem 6
 *      * tamanho:
 *        * tamanho nome
 *      * buf:
 *        * nome
 *    - topics
 *      * igual ao "topics" para utilizadores, mas apresenta também o número de
 *        mensagens persistentes (tempo de vida > 0)
 *      * tipo mensagem 7
 *    - show <tópico>
 *      * apresenta todas as mensagens persistentes de um tópico
 *      * tipo mensagem 8
 *      * tamanho:
 *        * tamanho tópico
 *      * buf:
 *        * tópico
 *    - lock / unlock <tópico>
 *      * bloqueia / desbloqueia o envio de novas mensagens para um tópico
 *      * tipo mensagem 9 / 10
 *      * tamanho:
 *        * tamanho tópico
 *      * buf:
 *        * tópico
 *    - close
 *      * termina a plataforma, ou seja, fecha o manager e todas as sessões de
 *        utilizadores
 *      * tipo mensagem 11
 *      * tamanho 0
 *      * buf vazio
 *
 **************************** MENSAGENS DO MANAGER ****************************
 *    Os nomes dos seguintes "comandos" são apenas ilustrativos da sua
 *    funcionalidade e não podem ser executados como comandos do feed. Servem
 *    apenas para descrever o formato das respostas dadas pelo manager ao feed
 *
 *    - msg
 *      * redireciona uma mensagem recebida a um utilizador. Estrutura identica
 *        ao comando "msg" dos utilizadores com exceção ao tipo de mensagem
 *      * tipo mensagem 12
 *    - remove user
 *      * quando um utilizador termina a sua sessão ou é excluido, esta mensagem
 *        é enviada a todos os utilizadores para indicar que utilizador saiu.
 *        Caso um utilizador receba esta mensagem e tenha um username identico
 *        ao indicado pela mesma, então termina a sua sessão
 *      * tipo mensagem 13
 *      * tamanho:
 *        * tamanho nome
 *      * buf:
 *        * nome
 *    - topics
 *      * por preencher
 *    - users
 *      * por preencher
 *    - locked topic
 *      * indica a um utilizador que o topico não permite novas mensagens
 *      * por preencher
 */
