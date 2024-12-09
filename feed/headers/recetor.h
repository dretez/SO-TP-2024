#ifndef FEED_RECETOR_H
#define FEED_RECETOR_H

#include <stdio.h>
#include <stdlib.h>

#include "../../include/communication.h"
#include "../../include/notificacoes.h"

#define NOTIF_SCS_SUB "Subscrição realizada com sucesso\n"
#define NOTIF_SCS_UNSUB "Dessubscrição realizada com sucesso\n"
#define NOTIF_ERR_TOPIC_LIST_FULL                                              \
  "Lista de tópicos cheia, o novo tópico não foi criado\n"

int processPacket(packet *p);
int processSucess(packet *p);
int processError(packet *p);

#endif // !FEED_RECETOR_H
