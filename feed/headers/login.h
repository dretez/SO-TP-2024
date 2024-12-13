#ifndef INCLUDE_HEADERS_LOGIN_H_
#define INCLUDE_HEADERS_LOGIN_H_

#include <stdio.h>

#include "../../include/communication.h"
#include "../../include/notificacoes.h"

int login(packet *p, char *nome, int fd, int fd_cli);

#endif // INCLUDE_HEADERS_LOGIN_H_
