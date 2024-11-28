#ifndef MANAGER_RECETOR_H
#define MANAGER_RECETOR_H
#include "../../include/communication.h"
#include "dados.h"

int processPacket(packet *p, managerData *d);

int answer(packet *p, managerData *d);

#endif // MANAGER_RECETOR_H
