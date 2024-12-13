#include "../headers/recetor.h"

int processPacket(packet *p, managerData *d) {
  switch (p->head.tipo_msg) {
  case P_TYPE_USER_HANDSHK: { // Novo utilizador
    switch (addUser(d, p->head.pid, p->buf)) {
    case 0:
      writeSucessPacket(p, P_SCS_HANDSHK);
      break;
    case 1:
      writeErrorPacket(p, P_ERR_ALREADY_LOGGED);
      break;
    case 2:
      writeErrorPacket(p, P_ERR_SRV_FULL);
      break;
    default:
      writeErrorPacket(p, P_ERR_GENERIC);
      break;
    }
    break;
  }

  case P_TYPE_USER_TOPIC: { // Pedido de lista de tópicos
    int offset = 0;

    // armazenar o # de topicos facilita a recessão deste packet do lado do feed
    memcpy(&p->buf[offset], &d->ntopics, sizeof(unsigned short));
    offset += sizeof(unsigned short);

    for (int i = 0; i < d->ntopics; i++) {
      strcpy(&p->buf[offset], d->topics[i].name);
      offset += strlen(d->topics[i].name) + 1;
    }
    p->head.tipo_msg = P_TYPE_MNGR_TOPIC;
    p->head.tam_msg = offset;
    break;
  }

  case P_TYPE_USER_MSG: { // Recebida mensagem para um tópico
    p->head.tipo_msg = P_TYPE_MNGR_MSG;
    break;
  }

  case P_TYPE_USER_SUB: { // Pedido de subscrição a um tópico
    if (getTopic(d->topics, d->ntopics, p->buf) == NULL &&
        addTopic(d, p->buf) == 1) {
      writeErrorPacket(p, P_ERR_TOPIC_LIST_FULL);
      break;
    }
    topic *topico = getTopic(d->topics, d->ntopics, p->buf);
    if (subscribeUser(topico, p->head.pid)) {
      writeErrorPacket(p, P_ERR_ALREADY_SUBBED);
      break;
    }
    break;
  }

  case P_TYPE_USER_UNSUB: { // Pedido de remoção de subscrição a um tópico
    topic *topico = getTopic(d->topics, d->ntopics, p->buf);
    if (topico == NULL) {
      writeErrorPacket(p, P_ERR_INVALID_TOPIC);
      break;
    }
    if (unsubscribeUser(topico, p->head.pid)) {
      writeErrorPacket(p, P_ERR_NOT_SUBBED);
      break;
    }
    writeSucessPacket(p, P_SCS_UNSUB);
    break;
  }

  case P_TYPE_USER_EXIT: { // Pedido de saída de um cliente feed
    char *uname = d->users[findUsr(d, p->head.pid)].name;
    if (uname == NULL) {
      writeErrorPacket(p, P_ERR_EXIT);
      break;
    }
    writeSingleValPacket(p, P_TYPE_MNGR_RMUSR, uname, strlen(uname) + 1);
    if (rmUser(d, p->head.pid)) {
      // falhou a remoção do utilizador
      writeErrorPacket(p, P_ERR_EXIT);
      break;
    }
    break;
  }

  case P_TYPE_ADMN_USERS: {
    for (int i = 0; i < d->nusers; i++)
      printf("%s\n", d->users[i].name);
    p->head.tipo_msg = P_TYPE_MNGR_NOANSW;
    break;
  }

  case P_TYPE_ADMN_RMUSR: {
    int upid = findUsrByName(d, p->buf);
    if (upid == -1) {
      printf("[ERRO] Utilizador não encontrado\n");
      p->head.tipo_msg = P_TYPE_MNGR_NOANSW;
      break;
    }
    if (rmUser(d, upid)) {
      printf("[ERRO] Falha ao remover utilizador\n");
      p->head.tipo_msg = P_TYPE_MNGR_NOANSW;
      break;
    }
    p->head.pid = upid;
    p->head.tipo_msg = P_TYPE_MNGR_FORCE_RMUSR;
    break;
  }

  case P_TYPE_ADMN_TOPIC: {
    for (int i = 0; i < d->ntopics; i++) {
      printf("%s - %d mensagens persistentes\n", d->topics[i].name,
             d->topics[i].nPersistMsgs);
    }
    p->head.tipo_msg = P_TYPE_MNGR_NOANSW;
    break;
  }

  case P_TYPE_ADMN_SHWTOPIC: {
    topic *t = getTopic(d->topics, d->ntopics, p->buf);
    if (t == NULL) {
      printf("[ERRO] Tópico desconhecido\n");
      p->head.tipo_msg = P_TYPE_MNGR_NOANSW;
      break;
    }
    for (int i = 0; i < t->nPersistMsgs; i++)
      printf(NOTIF_FEED_MSG, t->name, t->persistMsgs[i].uname,
             t->persistMsgs[i].msg);

    // print persistent msgs from a topic received from the buffer
    p->head.tipo_msg = P_TYPE_MNGR_NOANSW;
    break;
  }

  case P_TYPE_ADMN_LOCK:
  case P_TYPE_ADMN_UNLOCK: {
    topic *topico = getTopic(d->topics, d->ntopics, p->buf);
    if (getTopic(d->topics, d->ntopics, p->buf) == NULL) {
      printf("Tópico \"%s\" não existe", p->buf);
      break;
    }
    topico->lock = p->head.tipo_msg == P_TYPE_ADMN_LOCK ? 1 : 0;
    break;
  }

  case P_TYPE_ADMN_CLOSE: {
    writeEmptyPacket(p, P_TYPE_MNGR_CLOSE);
    break;
  }

  case P_TYPE_MNGR_DECREMENT: {
    for (int i = 0; i < d->ntopics; i++) {
      topic *t = &d->topics[i];
      for (int j = 0; j < t->nPersistMsgs; j++) {
        t->persistMsgs[j].lifetime--;
        if (t->persistMsgs[j].lifetime <= 0)
          rmPersistMsg(t, j);
      }
    }
    break;
  }

  default: { // Tipo de mensagem desconhecido
    writeErrorPacket(p, P_ERR_GENERIC);
    break;
  }
  }

  return 0;
}
