#include "../headers/recetor.h"

int processPacket(packet *p, managerData *d) {
  switch (p->head.tipo_msg) {
  case P_TYPE_USER_HANDSHK: { // Novo utilizador
    if (addUser(d, p->head.pid, p->buf)) {
      writeErrorPacket(p, P_ERR_GENERIC);
      break;
    }
    writeSucessPacket(p, P_SCS_GENERIC);
    break;
  }

  case P_TYPE_USER_TOPIC: { // Pedido de lista de tópicos
    p->head.tipo_msg = P_TYPE_MNGR_TOPIC;
    uint16_t offset;
    for (int i = 0, offset = 0; i < d->ntopics;
         offset += strlen(d->topics[i++].name) + 1)
      strcpy(&p->buf[offset], d->topics[i].name);
    p->head.tam_msg = offset;
    break;
  }

  case P_TYPE_USER_MSG: { // Recebida mensagem para um tópico
    int offset = 0, lifetime;
    memcpy(&lifetime, &p->buf[offset], sizeof(lifetime));
    offset += sizeof(lifetime);
    char *uname = &p->buf[offset];
    offset += strlen(uname) + 1;
    char *topic = &p->buf[offset];
    offset += strlen(topic) + 1;
    char *msg = &p->buf[offset];
    if (lifetime > 0)
      addPersistMsg(d, topic, uname, msg, lifetime);
    writeMsgPacket(p, P_TYPE_MNGR_MSG, -1, topic, uname, msg);
    break;
  }

  case P_TYPE_USER_SUB: { // Pedido de subscrição a um tópico
    topic *topico = getTopic(d->topics, d->ntopics, p->buf);
    if (topico == NULL && addTopic(d, p->buf) == 1) {
      writeErrorPacket(p, P_ERR_TOPIC_LIST_FULL);
      break;
    }
    if (subscribeUser(topico, p->head.pid)) {
      writeErrorPacket(p, P_ERR_ALREADY_SUBBED);
      break;
    }
    // TODO: enviar de volta todas as mensagens persistentes
    p->head.tipo_msg = P_TYPE_MNGR_SUCCESS;
    p->head.tam_msg = 0;
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
    // print utilizadores
    break;
  }

  case P_TYPE_ADMN_RMUSR: {
    // force remove a user
    break;
  }

  case P_TYPE_ADMN_TOPIC: {
    // print topics and nPersistMsgs
    break;
  }

  case P_TYPE_ADMN_SHWTOPIC: {
    // print persistent msgs from a topic received from the buffer
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
