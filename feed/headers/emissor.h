#ifndef FEED_EMISSOR_H
#define FEED_EMISSOR_H

#include "../../include/communication.h"
#include "../../include/globals.h"
#include "../../include/limites.h"
#include "../../include/notificacoes.h"

#define FEED_CMD_FMT_EXIT "exit\n"
#define FEED_CMD_FMT_TOPIC "topics\n"
#define FEED_CMD_FMT_SUB "subscribe <topico>\n"
#define FEED_CMD_FMT_UNSUB "unsubscribe <topico>\n"
#define FEED_CMD_FMT_MSG "msg <topico> <duracao> <mensagem>\n"

#define FEED_ERR_FMT "[ERRO] Formato esperado: "

int processCmd(packet *p, char *input, char *username);

#endif // !FEED_EMISSOR_H
