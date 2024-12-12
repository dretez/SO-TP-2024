#include "../headers/recetor.h"

void write2feed(packet *p, pid_t pid);

void sendPersistMsgs(packet *p, managerData *d, char *tname);

int answer(packet *p, managerData *d) {

  switch (p->head.tipo_msg) {
  case P_TYPE_MNGR_NOANSW:
    break;
  case P_TYPE_MNGR_MSG: {
    int offset = 0, lifetime;
    memcpy(&lifetime, &p->buf[offset], sizeof(lifetime));
    offset += sizeof(lifetime);

    char *uname = malloc(sizeof(char) * (strlen(&p->buf[offset] + 1)));
    if (uname == NULL) {
      writeErrorPacket(p, P_ERR_GENERIC);
      break;
    }
    strcpy(uname, &p->buf[offset]);
    offset += strlen(uname) + 1;

    char topico[TAM_NOME_TOPICO];
    strcpy(topico, &p->buf[offset]);
    if (getTopic(d->topics, d->ntopics, topico) == NULL &&
        addTopic(d, topico) == 1) {
      writeErrorPacket(p, P_ERR_TOPIC_LIST_FULL);
      free(uname);
      break;
    }
    topic *t = getTopic(d->topics, d->ntopics, topico);
    if (t->lock) {
      free(uname);
      writeErrorPacket(p, P_ERR_IS_LOCKED);
      break;
    }
    offset += strlen(topico) + 1;

    char msg[TAM_CORPO_MSG];
    strcpy(msg, &p->buf[offset]);

    if (lifetime > 0 && addPersistMsg(d, topico, uname, msg, lifetime) != 0) {
      // falha ao criar uma mensagem persistente
    }

    if (!subscribeUser(getTopic(d->topics, d->ntopics, topico), p->head.pid)) {
      writeSucessPacket(p, P_SCS_SUB);
      write2feed(p, p->head.pid);
    }

    writeMsgPacket(p, P_TYPE_MNGR_MSG, -1, topico, uname, msg);

    for (int i = 0; i < t->nsubs; i++) {
      write2feed(p, t->subs[i]);
    }

    free(uname);
    break;
  }

  case P_TYPE_MNGR_FORCE_RMUSR: {
    for (int i = 0; i < d->nusers; i++) {
      write2feed(p, d->users[i].pid);
    }
    writeSucessPacket(p, P_SCS_FORCE_EXIT);
    write2feed(p, p->head.pid);
    break;
  }

  case P_TYPE_MNGR_RMUSR: {
    for (int i = 0; i < d->nusers; i++) {
      write2feed(p, d->users[i].pid);
    }
    writeSucessPacket(p, P_SCS_EXIT);
    write2feed(p, p->head.pid);
    break;
  }

  case P_TYPE_USER_SUB: {
    sendPersistMsgs(p, d, p->buf);
    writeSucessPacket(p, P_SCS_SUB);
    write2feed(p, p->head.pid);
    break;
  }

  case P_TYPE_MNGR_CLOSE: {
    for (int i = 0; i < d->nusers; i++)
      write2feed(p, d->users[i].pid);
    return 1;
  }

  default: {
    write2feed(p, p->head.pid);
    break;
  }
  }
  return 0;
}

void write2feed(packet *p, pid_t pid) {
  char fifo_cli[30];
  sprintf(fifo_cli, FIFO_CLI, pid);
  int fd_cli = open(fifo_cli, O_WRONLY);

  write(fd_cli, p, packetSize(*p));

  close(fd_cli);
}

void sendPersistMsgs(packet *p, managerData *d, char *tname) {
  topic *t = getTopic(d->topics, d->ntopics, tname);
  for (int i = 0; i < t->nPersistMsgs; i++) {
    persistMsg *pmsg = &t->persistMsgs[i];
    writeMsgPacket(p, P_TYPE_MNGR_MSG, -1, t->name, pmsg->uname, pmsg->msg);
    write2feed(p, p->head.pid);
  }
}
