#ifndef SHARED_COMMS_H
#define SHARED_COMMS_H

#include "limites.h"
#include <limits.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

// Macros
#define maxval(type)                                                           \
  (1 << sizeof(type) * CHAR_BIT) - 1 // obtém o valor maximo permitido por type
                                     // ou o equivalente a colocar todos os
                                     // bits de type a 1

// #include "globals.h"

// Dados mensagens
#define TAM_BUF maxval(uint16_t)

// Códigos de erro
// #define P_ERR_
#define P_ERR_GENERIC 0
#define P_ERR_SRV_FULL 1
#define P_ERR_TOPIC_LIST_FULL 2
#define P_ERR_ALREADY_SUBBED 3
#define P_ERR_INVALID_TOPIC 4
#define P_ERR_NOT_SUBBED 5
#define P_ERR_EXIT 6

// Códigos de sucesso
// #define P_SCS_
#define P_SCS_GENERIC 0
#define P_SCS_SUB 1
#define P_SCS_UNSUB 2
#define P_SCS_EXIT 3
#define P_SCS_HANDSHK 4

// Tipos de mensagem
// mensagens do feed
#define P_TYPE_USER_TOPIC 0
#define P_TYPE_USER_MSG 1
#define P_TYPE_USER_SUB 2
#define P_TYPE_USER_UNSUB 3
#define P_TYPE_USER_EXIT 4
#define P_TYPE_USER_HANDSHK 19 // pacote enviado no login
// mensagens do manager: comandos do admin
#define P_TYPE_ADMN_USERS 5
#define P_TYPE_ADMN_RMUSR 6
#define P_TYPE_ADMN_TOPIC 7
#define P_TYPE_ADMN_SHWTOPIC 8
#define P_TYPE_ADMN_LOCK 9
#define P_TYPE_ADMN_UNLOCK 10
#define P_TYPE_ADMN_CLOSE 11
// mensagens do manager: respostas
#define P_TYPE_MNGR_MSG 12
#define P_TYPE_MNGR_RMUSR 13
#define P_TYPE_MNGR_TOPIC 14
#define P_TYPE_MNGR_LOCKD 15
#define P_TYPE_MNGR_USERS 16
#define P_TYPE_MNGR_ERROR 17
#define P_TYPE_MNGR_SUCCESS 18

typedef struct {
  pid_t pid;
  unsigned short tipo_msg;
  uint16_t tam_msg;
} packetHeader;

typedef struct {
  packetHeader head;
  char buf[TAM_BUF];
} packet;

size_t packetSize(packet p);

void writeEmptyPacket(packet *pack, short msgtype);

void writeSingleValPacket(packet *pack, short msgtype, char *value,
                          short valuesize);

void writeStringListPacket(packet *pack, short msgtype, char **strs,
                           int strsSize);

void writeMsgPacket(packet *pack, short msgtype, int msglifetime, char *topic,
                    char *username, char *msg);

void writeErrorPacket(packet *p, int errCode);

void writeSucessPacket(packet *p, int scsCode);

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
 *    - subscribe / unsubscribe <tópico>
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
 *    - topics usr
 *      * envia uma lista de tópicos
 *      * tipo mensagem 14
 *      * tamanho:
 *        * 1 int (4 bytes) +
 *        * n ints (n * 4 bytes) +
 *        * n tópicos (? bytes)
 *      * buf:
 *        * # de tópicos
 *        * tamanho tópico 1
 *        * tópico 1
 *        * ...
 *        * tamanho tópico n
 *        * tópico n
 *    - topics admin
 *      * idêntico ao topics usr, com a adicição do número de mensagens
 *        persistentes
 *      * tipo mensagem 15
 *      * buf:
 *        * # de tópicos
 *        * # msgs persistentes tópico 1
 *        * tamanho tópico 1
 *        * tópico 1
 *        * ...
 *        * # msgs persistentes tópico n
 *        * tamanho tópico n
 *        * tópico n
 *    - users
 *      * envia uma lista de utilizadores
 *      * tipo mensagem 16
 *      * tamanho:
 *        * 1 int (4 bytes) +
 *        * n ints (n * 4 bytes) +
 *        * n nomes (? bytes)
 *      * buf:
 *        * # de nomes
 *        * tamanho nome 1
 *        * nome 1
 *        * ...
 *        * tamanho nome n
 *        * nome n
 *    - locked topic
 *      * indica a um utilizador que o topico não permite novas mensagens
 *      * por preencher
 *    - error reading msg
 *      * avisa a um utilizador que ocorreu um erro no tratamento do comando /
 *        mensagem enviada
 *      * tipo mensagem 17
 *      * tamanho:
 *        * 1 int
 *      * buf:
 *        * código de erro
 *    - operation successful
 *      * para comandos que não esperam nenhuns dados do manager, esta mensagem
 *        serve para indicar que o comando foi executado com sucesso. Se esta
 *        mensagem não for recebida pelo feed que invocou o comando, assumme-se
 *        que houve um erro na comunicação com o manager.
 *      * tipo mensagem 18
 *      * tamanho 0
 *      * buf vazio
 ******************** FUNÇÕES DE PREENCHIMENTO DE MENSAGEM ********************
 *  Para melhor tratar as anteriores mensagens, serão criadas funções que
 *  auxiliem no preenchimento dos dados da mensagem, visto várias mensagens
 *  partilharem a mesma estrutura
 *
 *  fillEmptyMsg(msgtype)
 *    muitas mensagens possuem apenas o conteúdo do header, não enviando nada
 *    no buffer
 *
 *  fillListofStringsMsg(msgtype, ptrlistofstrs)
 *    algumas mensagens contém uma lista de strings, pelo que precisam de
 *    indicar: quantas strings contêm, o tamanho de cada string e a respetiva
 *    string
 *
 *  fillSingleValueMsg(msgtype, value)
 *    para quando é necessário enviar 1 valor, desce um int a uma string
 *
 *  fillFeedMsg(msgtype, msglifetime, topic, username, msg)
 *    preenche a mensagem com a estrutura previamente descrita de uma mensagem
 *    para os feeds
 */

#endif // !SHARED_COMMS_H
