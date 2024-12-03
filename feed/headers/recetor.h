#ifndef FEED_RECETOR_H
#define FEED_RECETOR_H

#include <stdio.h>

#include "../../include/communication.h"

int processPacket(packet *p);
int processSucess(packet *p);
int processError(packet *p);

#endif // !FEED_RECETOR_H
