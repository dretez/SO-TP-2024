#include "../headers/recetor.h"

void write2feed(packet *p, pid_t pid);

int answer(packet *p, managerData *d) {

  switch (p->head.tipo_msg) {
  case P_TYPE_MNGR_NOANSW:
    break;
  case P_TYPE_MNGR_MSG: {
    // encontra o início do nome do tópico no buffer do packet
    int offset = sizeof(int);
    offset += strlen(&p->buf[offset]) + 1;

    char topico[TAM_NOME_TOPICO];
    strcpy(topico, &p->buf[offset]);

    if (getTopic(d->topics, d->ntopics, topico) == NULL)
      addTopic(d, topico);
    topic *t = getTopic(d->topics, d->ntopics, topico);

    for (int i = 0; i < t->nsubs; i++) {
      write2feed(p, t->subs[i]);
    }
    break;
  }

  case P_TYPE_MNGR_RMUSR: {
    writeSucessPacket(p, P_SCS_EXIT);
    write2feed(p, p->head.pid);
    p->head.tipo_msg = P_TYPE_MNGR_RMUSR;
    p->head.tam_msg = strlen(p->buf) + 1;
    for (int i = 0; i < d->nusers; i++) {
      write2feed(p, d->users[i].pid);
    }
    break;
  }

  case P_TYPE_USER_SUB: {
    topic *t = getTopic(d->topics, d->ntopics, p->buf);
    for (int i = 0; i < t->nPersistMsgs; i++) {
      persistMsg *pmsg = &t->persistMsgs[i];
      writeMsgPacket(p, P_TYPE_MNGR_MSG, -1, t->name, pmsg->uname, pmsg->msg);
      write2feed(p, p->head.pid);
    }
    p->head.tipo_msg = P_TYPE_MNGR_SUCCESS;
    p->head.tam_msg = 0;
    write2feed(p, p->head.pid);
    break;
  }

  default: {
    write2feed(p, p->head.pid);
  }
  }
  return 0;
}

void write2feed(packet *p, pid_t pid) {
  char fifo_cli[30];
  sprintf(fifo_cli, FIFO_CLI, pid);
  int fd_cli = open(fifo_cli, O_WRONLY);

  int res = write(fd_cli, p, packetSize(*p));

  close(fd_cli);
}
