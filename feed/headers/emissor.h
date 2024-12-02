#ifndef FEED_EMISSOR_H
#define FEED_EMISSOR_H

#include "../../include/communication.h"
#include "../../include/globals.h"
#include "../../include/limites.h"

#define FEED_CMD_FMT_EXIT "exit\0"
#define FEED_CMD_FMT_TOPIC "topics\0"
#define FEED_CMD_FMT_SUB "subscribe <topico>\0"
#define FEED_CMD_FMT_UNSUB "unsubscribe <topico>\0"
#define FEED_CMD_FMT_MSG "msg <topico> <duracao> <mensagem>\0"

#define FEED_ERR_FMT "[ERRO] Formato esperado: \0"

int processCmd(packet *p, char *input, char *username);

#endif // !FEED_EMISSOR_H
