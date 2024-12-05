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
    uint16_t offset = 0;

    // armazenar o # de topicos facilita a recessão deste packet do lado do feed
    memcpy(&p->buf[offset], &d->ntopics, sizeof(unsigned short));
    // printf("%x\n", d->ntopics);
    // printf("%x\n", *((unsigned short *)&p->buf[offset]));
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
    offset += strlen(topico) + 1;
    char msg[TAM_CORPO_MSG];
    strcpy(msg, &p->buf[offset]);
    if (getTopic(d->topics, d->ntopics, topico) == NULL &&
        addTopic(d, topico) == 1) {
      writeErrorPacket(p, P_ERR_TOPIC_LIST_FULL);
      break;
    }
    subscribeUser(getTopic(d->topics, d->ntopics, topico), p->head.pid);
    if (lifetime > 0 && addPersistMsg(d, topico, uname, msg, lifetime) != 0) {
      // falha ao criar uma mensagem persistente
    }
    writeMsgPacket(p, P_TYPE_MNGR_MSG, -1, topico, uname, msg);
    break;
  }

  case P_TYPE_USER_SUB: { // Pedido de subscrição a um tópico
    topic *topico = getTopic(d->topics, d->ntopics, p->buf);
    if (topico == NULL && addTopic(d, p->buf) == 1) {
      writeErrorPacket(p, P_ERR_TOPIC_LIST_FULL);
      break;
    }
    topico = getTopic(d->topics, d->ntopics, p->buf);
    if (subscribeUser(topico, p->head.pid)) {
      writeErrorPacket(p, P_ERR_ALREADY_SUBBED);
      break;
    }
    break;
  }

  case P_TYPE_USER_UNSUB: { // Pedido de remoção de subscrição a um tópico
    p->head.tam_msg = 0;
    topic *topico = getTopic(d->topics, d->ntopics, p->buf);
    if (topico == NULL) {
      writeErrorPacket(p, P_ERR_INVALID_TOPIC);
      break;
    }
    if (unsubscribeUser(topico, p->head.pid)) {
      writeErrorPacket(p, P_ERR_NOT_SUBBED);
      break;
    }
    p->head.tipo_msg = P_TYPE_MNGR_SUCCESS;
    break;
  }

  case P_TYPE_USER_EXIT: { // Pedido de saída de um cliente feed
    if (rmUser(d, p->head.pid)) {
      // falhou a remoção do utilizador
      writeErrorPacket(p, P_ERR_EXIT);
      break;
    }
    p->head.tipo_msg = P_TYPE_MNGR_RMUSR;
    break;
  }

  case P_TYPE_ADMN_USERS: {
    for (int i = 0; i < d->nusers; i++)
      printf("%s\n", d->users[i].name);
    p->head.tipo_msg = P_TYPE_MNGR_NOANSW;
    break;
  }

  case P_TYPE_ADMN_RMUSR: {
    // force remove a user
    break;
  }

  case P_TYPE_ADMN_TOPIC: {
    for (int i = 0; i < d->ntopics; i++)
      printf("%s - %d mensagens persistentes\n", d->topics[i].name,
             d->topics[i].nPersistMsgs);
    p->head.tipo_msg = P_TYPE_MNGR_NOANSW;
    break;
  }

  case P_TYPE_ADMN_SHWTOPIC: {
    topic *t = getTopic(d->topics, d->ntopics, p->buf);
    for (int i = 0; i < t->nPersistMsgs; i++)
      printf(NOTIF_FEED_MSG, t->name, t->persistMsgs[i].uname,
             t->persistMsgs[i].msg);

    // print persistent msgs from a topic received from the buffer
    p->head.tipo_msg = P_TYPE_MNGR_NOANSW;
    break;
  }

  case P_TYPE_ADMN_LOCK: {
    // lock topic received from buffer and inform subbed users
    break;
  }

  case P_TYPE_ADMN_UNLOCK: {
    // unlock topic received from buffer and inform subbed users
    break;
  }

  case P_TYPE_ADMN_CLOSE: {
    // inform all users of the manager's closure, save data and close
    break;
  }

  default: { // Tipo de mensagem desconhecido
    writeErrorPacket(p, P_ERR_GENERIC);
    break;
  }
  }

  return 0;
}
