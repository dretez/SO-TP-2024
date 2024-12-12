#ifndef INCLUDE_HEADERS_DADOSAUX_H_
#define INCLUDE_HEADERS_DADOSAUX_H_

#include "../../include/communication.h"
#include "dados.h"

/* Recupera mensagens persistentes armazenadas por uma sessão anterior */
void loadPMsgs(managerData *d, FILE *f);

/* Esvazia a lista de utilizadores */
void initUsrs(user *u);

/* Inicializa a lista de tópicos */
void initTopics(topic *t);
void swapTopic(topic *t1, topic *t2);
/* Copia todos os dados de um tópico src para um tópico dest, perdendo este, por
 * sua vez, todos os seus dados originais, passando também a partilhar as
 * mensagens do tópico src (uma alteração a uma mensagem num tópico também
 * altera a mesma mensagem no outro) */
void copyTopic(topic *dest, topic *src);
/* Apaga os dados de um tópico */
void deleteTopic(topic *t);

int swapPersistMsg(persistMsg *msg1, persistMsg *msg2);
int cpyPersistMsg(persistMsg *msg1, persistMsg *msg2);

#endif // INCLUDE_HEADERS_DADOSAUX_H_
