#include "../headers/recetor.h"

void write2feed(packet *p, pid_t pid);

int answer(packet *p, managerData *d) {

  switch (p->head.tipo_msg) {
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
  }
  case P_TYPE_MNGR_RMUSR: {
    // envia a todos os utilizadores ligados
    // d.users contém a lista de utilizadores
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
